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
#pragma once
#include <SofaBaseTopology/TopologyData.h>
#include <SofaBaseTopology/TopologyDataEngine.inl>
#include <SofaBaseTopology/TopologyDataHandler.inl>

namespace sofa::component::topology
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////   Generic Topology Data Implementation   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////



template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::createTopologicalEngine(sofa::core::topology::BaseMeshTopology *_topology, sofa::component::topology::TopologyDataHandler<TopologyElementType,VecT>* _topologyHandler, bool deleteHandler)
{
    this->m_topology = _topology;
    if (_topology && dynamic_cast<sofa::core::topology::TopologyContainer*>(_topology))
    {
        this->m_topologicalEngine = sofa::core::objectmodel::New<TopologyDataEngine<VecT> >((sofa::component::topology::TopologyData<TopologyElementType, VecT>*)this, _topology, _topologyHandler);
        this->m_topologicalEngine->setNamePrefix(std::string(sofa::core::topology::TopologyElementInfo<TopologyElementType>::name()) + std::string("Engine_"));
        if (this->getOwner() && dynamic_cast<sofa::core::objectmodel::BaseObject*>(this->getOwner())) dynamic_cast<sofa::core::objectmodel::BaseObject*>(this->getOwner())->addSlave(this->m_topologicalEngine.get());
        this->m_topologicalEngine->init();
        this->linkToElementDataArray((TopologyElementType*)nullptr);
        msg_info(this->getOwner())<<"TopologyData: " << this->getName() << " initialized with dynamic " << _topology->getClassName() << "Topology." ;
        if (deleteHandler && _topologyHandler) m_topologyHandler = _topologyHandler;
    }
    else if (_topology)
    {
        msg_info(this->getOwner())<<"TopologyData: " << this->getName() << " initialized with static " << _topology->getClassName() << " Topology." ;
        if (deleteHandler && _topologyHandler) delete _topologyHandler;
    }
    else
    {
        msg_info(this->getOwner())<<"TopologyData: No Topology given to " << this->getName() << " to createTopologicalEngine. Topological changes will be disabled." ;
        if (deleteHandler && _topologyHandler) delete _topologyHandler;
    }
}


template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::createTopologicalEngine(sofa::core::topology::BaseMeshTopology *_topology)
{
    this->m_topologyHandler = new TopologyDataHandler<TopologyElementType, VecT>(this);
    createTopologicalEngine(_topology, this->m_topologyHandler);
}


template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::registerTopologicalData()
{
    if (this->m_topologicalEngine)
        this->m_topologicalEngine->registerTopology();
    else if (!this->m_topology)
        msg_info(this->getOwner()) << "TopologyData: " << this->getName() << " has no engine. Topological changes will be disabled. Use createTopologicalEngine method before registerTopologicalData to allow topological changes." ;
}

template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::addInputData(sofa::core::objectmodel::BaseData *_data)
{
    if (this->m_topologicalEngine)
        this->m_topologicalEngine->addInput(_data);
    else if (!this->m_topology)
        msg_info(this->getOwner()) <<"Warning: TopologyData: " << this->getName() << " has no engine. Use createTopologicalEngine function before addInputData." ;
}




/// Method used to link Data to point Data array, using the engine's method
template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::linkToPointDataArray()
{
    if(m_topologicalEngine)
        m_topologicalEngine->linkToPointDataArray();
}

/// Method used to link Data to edge Data array, using the engine's method
template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::linkToEdgeDataArray()
{
    if(m_topologicalEngine)
        m_topologicalEngine->linkToEdgeDataArray();
}

/// Method used to link Data to triangle Data array, using the engine's method
template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::linkToTriangleDataArray()
{
    if(m_topologicalEngine)
        m_topologicalEngine->linkToTriangleDataArray();
}

/// Method used to link Data to quad Data array, using the engine's method
template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::linkToQuadDataArray()
{
    if(m_topologicalEngine)
        m_topologicalEngine->linkToQuadDataArray();
}

/// Method used to link Data to tetrahedron Data array, using the engine's method
template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::linkToTetrahedronDataArray()
{
    if(m_topologicalEngine)
        m_topologicalEngine->linkToTetrahedronDataArray();
}

/// Method used to link Data to hexahedron Data array, using the engine's method
template <typename TopologyElementType, typename VecT>
void TopologyData <TopologyElementType, VecT>::linkToHexahedronDataArray()
{
    if(m_topologicalEngine)
        m_topologicalEngine->linkToHexahedronDataArray();
}


} //namespace sofa::component::topology
