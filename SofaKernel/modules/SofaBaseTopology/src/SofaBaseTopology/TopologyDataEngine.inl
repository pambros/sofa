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
#include <SofaBaseTopology/TopologyDataEngine.h>

#include <SofaBaseTopology/TetrahedronSetTopologyContainer.h>
#include <SofaBaseTopology/HexahedronSetTopologyContainer.h>
#include <sofa/helper/AdvancedTimer.h>

namespace sofa::component::topology
{

template <typename VecT>
TopologyDataEngine< VecT>::TopologyDataEngine(t_topologicalData *_topologicalData,
        sofa::core::topology::BaseMeshTopology *_topology,
        sofa::core::topology::TopologyHandler *_topoHandler) :
    m_topologicalData(_topologicalData),
    m_topology(nullptr),
    m_topoHandler(_topoHandler),
    m_pointsLinked(false), m_edgesLinked(false), m_trianglesLinked(false),
    m_quadsLinked(false), m_tetrahedraLinked(false), m_hexahedraLinked(false)
{
    m_topology =  dynamic_cast<sofa::core::topology::TopologyContainer*>(_topology);

    if (m_topology == nullptr)
        msg_error() << "Topology is not dynamic";

    if (m_topoHandler == nullptr)
        msg_error() << "Topology Handler not available";
}

template <typename VecT>
void TopologyDataEngine< VecT>::init()
{
    // A pointData is by default child of positionSet Data
    //this->linkToPointDataArray();  // already done while creating engine

    // Name creation
    if (m_prefix.empty()) m_prefix = "TopologyEngine_";
    m_data_name = this->m_topologicalData->getName();
    this->addOutput(this->m_topologicalData);

    sofa::core::topology::TopologyEngine::init();

    // Register Engine in containter list
    //if (m_topology)
    //   m_topology->addTopologyEngine(this);
    //this->registerTopology(m_topology);
}


template <typename VecT>
void TopologyDataEngine< VecT>::reinit()
{
    this->update();
}


template <typename VecT>
void TopologyDataEngine< VecT>::doUpdate()
{
    std::string msg = this->name.getValue() + " - doUpdate: Nbr changes: " + std::to_string(m_changeList.getValue().size());
    sofa::helper::AdvancedTimer::stepBegin(msg.c_str());
    this->ApplyTopologyChanges();
    sofa::helper::AdvancedTimer::stepEnd(msg.c_str());
}


template <typename VecT>
void TopologyDataEngine< VecT>::registerTopology(sofa::core::topology::BaseMeshTopology *_topology)
{
    m_topology =  dynamic_cast<sofa::core::topology::TopologyContainer*>(_topology);

    if (m_topology == nullptr)
    {
        msg_error() <<"Topology: " << _topology->getName() << " is not dynamic, topology engine on Data '" << m_data_name << "' won't be registered.";
        return;
    }
    else
        m_topology->addTopologyEngine(this);
}


template <typename VecT>
void TopologyDataEngine< VecT>::registerTopology()
{
    if (m_topology == nullptr)
    {
        msg_error() << "Current topology is not dynamic, topology engine on Data '" << m_data_name << "' won't be registered.";
        return;
    }
    else
        m_topology->addTopologyEngine(this);
}


template <typename VecT>
void TopologyDataEngine< VecT>::ApplyTopologyChanges()
{
    // Rentre ici la premiere fois aussi....
    if(m_topoHandler)
    {
        m_topoHandler->ApplyTopologyChanges(m_changeList.getValue(), m_topology->getNbPoints());

        m_changeList.endEdit();
    }
}


/// Function to link DataEngine with Data array from topology
template <typename VecT>
void TopologyDataEngine< VecT>::linkToPointDataArray()
{
    if (m_pointsLinked) // avoid second registration
        return;

    sofa::component::topology::PointSetTopologyContainer* _container = dynamic_cast<sofa::component::topology::PointSetTopologyContainer*>(m_topology);

    if (_container == nullptr)
    {
        msg_error() << "Owner topology can't be cast as PointSetTopologyContainer, Data '" << m_data_name << "' won't be linked to Point Data Array.";
        return;
    }

    (_container->getPointDataArray()).addOutput(this);
    m_pointsLinked = true;
}


template <typename VecT>
void TopologyDataEngine< VecT>::linkToEdgeDataArray()
{
    if (m_edgesLinked) // avoid second registration
        return;

    sofa::component::topology::EdgeSetTopologyContainer* _container = dynamic_cast<sofa::component::topology::EdgeSetTopologyContainer*>(m_topology);

    if (_container == nullptr)
    {
        msg_error() << "Owner topology can't be cast as EdgeSetTopologyContainer, Data '" << m_data_name << "' won't be linked to Edge Data Array.";
        return;
    }

    (_container->getEdgeDataArray()).addOutput(this);
    m_edgesLinked = true;
}


template <typename VecT>
void TopologyDataEngine< VecT>::linkToTriangleDataArray()
{
    if (m_trianglesLinked) // avoid second registration
        return;

    sofa::component::topology::TriangleSetTopologyContainer* _container = dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer*>(m_topology);

    if (_container == nullptr)
    {
        msg_error() << "Owner topology can't be cast as TriangleSetTopologyContainer, Data '" << m_data_name << "' won't be linked to Triangle Data Array.";
        return;
    }

    (_container->getTriangleDataArray()).addOutput(this);
    m_trianglesLinked = true;
}


template <typename VecT>
void TopologyDataEngine< VecT>::linkToQuadDataArray()
{
    if (m_quadsLinked) // avoid second registration
        return;

    sofa::component::topology::QuadSetTopologyContainer* _container = dynamic_cast<sofa::component::topology::QuadSetTopologyContainer*>(m_topology);

    if (_container == nullptr)
    {
        msg_error() << "Owner topology can't be cast as QuadSetTopologyContainer, Data '" << m_data_name << "' won't be linked to Quad Data Array.";
        return;
    }

    (_container->getQuadDataArray()).addOutput(this);
    m_quadsLinked = true;
}


template <typename VecT>
void TopologyDataEngine< VecT>::linkToTetrahedronDataArray()
{
    if (m_tetrahedraLinked) // avoid second registration
        return;

    sofa::component::topology::TetrahedronSetTopologyContainer* _container = dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer*>(m_topology);

    if (_container == nullptr)
    {
        msg_error() << "Owner topology can't be cast as TetrahedronSetTopologyContainer, Data '" << m_data_name << "' won't be linked to Tetrahedron Data Array.";
        return;
    }

    (_container->getTetrahedronDataArray()).addOutput(this);
    m_tetrahedraLinked = true;
}


template <typename VecT>
void TopologyDataEngine< VecT>::linkToHexahedronDataArray()
{
    if (m_hexahedraLinked) // avoid second registration
        return;

    sofa::component::topology::HexahedronSetTopologyContainer* _container = dynamic_cast<sofa::component::topology::HexahedronSetTopologyContainer*>(m_topology);

    if (_container == nullptr)
    {
        msg_error() << "Owner topology can't be cast as HexahedronSetTopologyContainer, Data '" << m_data_name << "' won't be linked to Hexahedron Data Array.";
        return;
    }

    (_container->getHexahedronDataArray()).addOutput(this);
    m_hexahedraLinked = true;
}

} //namespace sofa::component::topology
