/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <SofaMeshCollision/LineLocalMinDistanceFilter.h>
#include <SofaBaseMechanics/MechanicalObject.h>
#include <SofaBaseTopology/TopologyData.inl>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/core/ObjectFactory.h>


namespace sofa::component::collision
{


LineInfo::LineInfo(LocalMinDistanceFilter *lmdFilters)
    : InfoFilter(lmdFilters)
    , m_computedRightAngleCone(0.0)
    , m_computedLeftAngleCone(0.0)
    , m_twoTrianglesAroundEdge(false)
{
}

void LineInfo::buildFilter(Index edge_index)
{
    using sofa::helper::vector;
    using sofa::core::topology::BaseMeshTopology;

    bool debug=false;

    if (edge_index == sofa::InvalidID)
        debug=true;

    BaseMeshTopology* bmt = this->base_mesh_topology;

    const Edge &e =  bmt->getEdge(edge_index);

    const sofa::defaulttype::Vector3 &pt1 = (*this->position_filtering)[e[0]];
    const sofa::defaulttype::Vector3 &pt2 = (*this->position_filtering)[e[1]];

    msg_info_when(debug, "LineInfo") <<"pt1: "<<pt1<<"  - pt2: "<<pt2;

    m_lineVector = pt2 - pt1;
    m_lineVector.normalize();

    const auto& trianglesAroundEdge = bmt->getTrianglesAroundEdge(edge_index);

    msg_info_when(debug, "LineInfo") <<"trianglesAroundEdge: "<<trianglesAroundEdge<<"  -";

    // filter if there are two triangles around the edge
    if (trianglesAroundEdge.size() == 1)
    {
        msg_info_when(debug, "LineInfo") <<"TODO : validity for segment on a single triangle";
    }

    // filter if there are two triangles around the edge
    if (trianglesAroundEdge.size() != 2)
    {
        m_twoTrianglesAroundEdge = false;
        return;
    }

    const sofa::helper::vector<sofa::defaulttype::Vector3>& x = *this->position_filtering;



    // which triangle is left ?
    const Triangle& triangle0 = bmt->getTriangle(trianglesAroundEdge[0]);
    bool triangle0_is_left=false;
    if ( (e[0]==triangle0[0]&&e[1]==triangle0[1]) || (e[0]==triangle0[1]&&e[1]==triangle0[2]) || (e[0]==triangle0[2]&&e[1]==triangle0[0]) )
        triangle0_is_left=true;

    // compute the normal of the triangle situated on the right
    const BaseMeshTopology::Triangle& triangleRight = triangle0_is_left ? bmt->getTriangle(trianglesAroundEdge[1]): bmt->getTriangle(trianglesAroundEdge[0]);
    sofa::defaulttype::Vector3 n1 = cross(x[triangleRight[1]] - x[triangleRight[0]], x[triangleRight[2]] - x[triangleRight[0]]);
    n1.normalize();
    m_nMean = n1;
    m_triangleRight = cross(n1, m_lineVector);
    m_triangleRight.normalize(); // necessary ?

    // compute the normal of the triangle situated on the left
    const BaseMeshTopology::Triangle& triangleLeft = triangle0_is_left ? bmt->getTriangle(trianglesAroundEdge[0]): bmt->getTriangle(trianglesAroundEdge[1]);
    sofa::defaulttype::Vector3 n2 = cross(x[triangleLeft[1]] - x[triangleLeft[0]], x[triangleLeft[2]] - x[triangleLeft[0]]);
    n2.normalize();
    m_nMean += n2;
    m_triangleLeft = cross(m_lineVector, n2);
    m_triangleLeft.normalize(); // necessary ?

    m_nMean.normalize();

    // compute the angle for the cone to filter contacts using the normal of the triangle situated on the right
    m_computedRightAngleCone = (m_nMean * m_triangleRight) * m_lmdFilters->getConeExtension();
    if(debug)
        msg_info_when(debug, "LineInfo") << "m_nMean: "<<m_nMean<<" - m_triangleRight:"<<m_triangleRight<<" - m_triangleLeft:"<<m_triangleLeft;
    if (m_computedRightAngleCone < 0)
    {
        m_computedRightAngleCone = 0.0;
    }
    m_computedRightAngleCone += m_lmdFilters->getConeMinAngle();
    if( debug)
        msg_info_when(debug, "LineInfo") <<"m_computedRightAngleCone :"<<m_computedRightAngleCone;

    // compute the angle for the cone to filter contacts using the normal of the triangle situated on the left
    m_computedLeftAngleCone = (m_nMean * m_triangleLeft) * m_lmdFilters->getConeExtension();
    if (m_computedLeftAngleCone < 0)
    {
        m_computedLeftAngleCone = 0.0;
    }
    m_computedLeftAngleCone += m_lmdFilters->getConeMinAngle();
    if( debug)
        msg_info_when(debug, "LineInfo")<<"m_computedLeftAngleCone :"<<m_computedRightAngleCone;


    setValid();
}

bool LineInfo::validate(const Index edge_index, const defaulttype::Vector3& PQ)
{
    bool debug=false;

    if (edge_index == sofa::InvalidID)
        debug=true;

    if (isValid())
    {
        msg_info_when(debug, "LineInfo") << "Line " << edge_index << " is valid";

        if (m_twoTrianglesAroundEdge)
        {
            msg_info_when(debug, "LineInfo") << "m_triangleRight :" << m_triangleRight << "  - m_triangleLeft" << m_triangleLeft;
            msg_info_when(debug, "LineInfo") <<"m_twoTrianglesAroundEdge ok tests: "<< (m_nMean * PQ)<<"<0 ?  - "<<m_triangleRight * PQ <<" < "<<-m_computedRightAngleCone * PQ.norm()<<" ?  - " <<m_triangleLeft * PQ <<" < "<<-m_computedLeftAngleCone * PQ.norm()<<" ?";

            if ((m_nMean * PQ) < 0)
                return false;

            if (m_triangleRight * PQ < -m_computedRightAngleCone * PQ.norm())
                return false;

            if (m_triangleLeft * PQ < -m_computedLeftAngleCone * PQ.norm())
                return false;
        }
        else
        {
            sofa::defaulttype::Vector3 PQnormalized = PQ;
            PQnormalized.normalize();

            if (fabs(dot(m_lineVector, PQnormalized)) > m_lmdFilters->getConeMinAngle() + 0.001)		// dot(AB,n1) should be equal to 0
            {
                // means that proximity was detected with a null determinant
                // in function computeIntersection
                return false;
            }
        }

        return true;
    }
    else
    {
        msg_info_when(debug, "LineInfo") <<"Line "<<edge_index<<" is no valid ------------ build";

        buildFilter(edge_index);
        return validate(edge_index, PQ);
    }
}

LineLocalMinDistanceFilter::LineLocalMinDistanceFilter()
    : m_pointInfo(initData(&m_pointInfo, "pointInfo", "point filter data"))
    , m_lineInfo(initData(&m_lineInfo, "lineInfo", "line filter data"))
    , pointInfoHandler(nullptr)
    , lineInfoHandler(nullptr)
    , bmt(nullptr)
{
}

LineLocalMinDistanceFilter::~LineLocalMinDistanceFilter()
{
    if (pointInfoHandler) delete pointInfoHandler;
    if (lineInfoHandler) delete lineInfoHandler;
}

void LineLocalMinDistanceFilter::init()
{
    this->bmt = getContext()->getMeshTopology();

    if (bmt != nullptr)
    {
        helper::vector< PointInfo >& pInfo = *(m_pointInfo.beginEdit());
        pInfo.resize(bmt->getNbPoints());
        m_pointInfo.endEdit();

        pointInfoHandler = new PointInfoHandler(this,&m_pointInfo);
        m_pointInfo.createTopologicalEngine(bmt, pointInfoHandler);
        m_pointInfo.registerTopologicalData();

        helper::vector< LineInfo >& lInfo = *(m_lineInfo.beginEdit());
        lInfo.resize(bmt->getNbEdges());
        m_lineInfo.endEdit();

        lineInfoHandler = new LineInfoHandler(this,&m_lineInfo);
        m_lineInfo.createTopologicalEngine(bmt, lineInfoHandler);
        m_lineInfo.registerTopologicalData();
    }
}



void LineLocalMinDistanceFilter::PointInfoHandler::applyCreateFunction(Index /*pointIndex*/, PointInfo &pInfo, const sofa::helper::vector<Index> &, const sofa::helper::vector< double >&)
{
    const LineLocalMinDistanceFilter *lLMDFilter = this->f;
    pInfo.setLMDFilters(lLMDFilter);

    sofa::core::topology::BaseMeshTopology * bmt = lLMDFilter->bmt; //getContext()->getTopology();
    pInfo.setBaseMeshTopology(bmt);
    /////// TODO : template de la classe
    component::container::MechanicalObject<sofa::defaulttype::Vec3Types>*  mstateVec3d= dynamic_cast<component::container::MechanicalObject<sofa::defaulttype::Vec3Types>*>(lLMDFilter->getContext()->getMechanicalState());
    if(mstateVec3d != nullptr)
    {
        pInfo.setPositionFiltering(&mstateVec3d->read(core::ConstVecCoordId::position())->getValue());
    }
}



void LineLocalMinDistanceFilter::LineInfoHandler::applyCreateFunction(Index /*edgeIndex*/, LineInfo &lInfo, const core::topology::BaseMeshTopology::Edge&, const sofa::helper::vector<Index> &, const sofa::helper::vector< double >&)
{
    const LineLocalMinDistanceFilter *lLMDFilter = this->f;
    lInfo.setLMDFilters(lLMDFilter);
    //
    sofa::core::topology::BaseMeshTopology * bmt = lLMDFilter->bmt; //getContext()->getTopology();
    lInfo.setBaseMeshTopology(bmt);


    /////// TODO : template de la classe
    component::container::MechanicalObject<sofa::defaulttype::Vec3Types>*  mstateVec3d= dynamic_cast<component::container::MechanicalObject<sofa::defaulttype::Vec3Types>*>(lLMDFilter->getContext()->getMechanicalState());
    if(mstateVec3d != nullptr)
    {
        lInfo.setPositionFiltering(&mstateVec3d->read(core::ConstVecCoordId::position())->getValue());
    }
}

bool LineLocalMinDistanceFilter::validPoint(const int pointIndex, const defaulttype::Vector3 &PQ)
{
    PointInfo & Pi = m_pointInfo[pointIndex];
    if(this->isRigid())
    {
        // filter is precomputed in the rest position
        defaulttype::Vector3 PQtest;
        PQtest = pos->getOrientation().inverseRotate(PQ);
        return Pi.validate(pointIndex,PQtest);
    }

    return Pi.validate(pointIndex,PQ);
}

bool LineLocalMinDistanceFilter::validLine(const int /*lineIndex*/, const defaulttype::Vector3 &/*PQ*/)
{
    return true;
}


int LineLocalMinDistanceFilterClass = core::RegisterObject("This class manages Line collision models cones filters computations and updates.")
        .add< LineLocalMinDistanceFilter >()
        ;

} //namespace sofa::component::collision
