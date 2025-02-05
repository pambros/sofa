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
#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject ;

#include <sofa/core/objectmodel/BaseNode.h>
using sofa::core::objectmodel::BaseNode ;

#include <sofa/core/objectmodel/BaseLink.h>
using sofa::core::objectmodel::BaseLink ;

#include <sofa/core/objectmodel/Link.h>
using sofa::core::objectmodel::SingleLink;
using sofa::core::objectmodel::MultiLink;

#include <sofa/helper/testing/BaseTest.h>
using sofa::helper::testing::BaseTest ;

#include "BaseLink_test.h"

using SingleLinkImplementations = ::testing::Types<
    SingleLink<BaseObject, BaseObject, BaseLink::FLAG_STOREPATH>,
    SingleLink<BaseObject, BaseObject, BaseLink::FLAG_MULTILINK>>;
INSTANTIATE_TYPED_TEST_SUITE_P(SingleLink, BaseLinkTests, SingleLinkImplementations);


using MultiLinkImplementations = ::testing::Types<
    MultiLink<BaseObject, BaseObject, BaseLink::FLAG_STOREPATH>,
    MultiLink<BaseObject, BaseObject, BaseLink::FLAG_MULTILINK>
    >;
INSTANTIATE_TYPED_TEST_SUITE_P(MultiLink, BaseLinkTests, MultiLinkImplementations);

