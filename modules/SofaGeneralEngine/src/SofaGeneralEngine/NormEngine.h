 
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
#include <SofaGeneralEngine/config.h>

#include <sofa/core/DataEngine.h>
#include <sofa/defaulttype/Vec.h>

namespace sofa::component::engine
{

/// convert a vector of Vecs in a vector of their l-norms
template <class TDataType>
class NormEngine : public core::DataEngine
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(NormEngine,TDataType),core::DataEngine);

    typedef TDataType DataType;
    typedef typename DataType::value_type Real;
    typedef helper::vector<DataType> VecData;
    typedef helper::vector<Real> VecReal;

    NormEngine();

    ~NormEngine() override {}

    void init() override;

    void reinit() override;

    void doUpdate() override;

    /// Returns the sofa template name. By default the name of the c++ class signature is exposed...
    /// so we need to override that by implementing GetCustomTemplateName() function
    /// More details on the name customization infrastructure is in NameDecoder.h
    static const std::string GetCustomTemplateName()
    {
        return defaulttype::DataTypeInfo<TDataType>::name();
    }


protected:

    Data<VecData> d_input; ///< input array of 3d points
    Data<VecReal> d_output; ///< output array of scalar norms
    Data<int> d_normType; ///< The type of norm. Use a negative value for the infinite norm.

};

#if  !defined(SOFA_COMPONENT_ENGINE_NORMENGINE_CPP)
extern template class SOFA_SOFAGENERALENGINE_API NormEngine<defaulttype::Vec3d>;

#endif

} //namespace sofa::component::engine
