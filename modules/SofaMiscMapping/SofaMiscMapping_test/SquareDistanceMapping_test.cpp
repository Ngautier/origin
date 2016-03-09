/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2016 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <SofaTest/Mapping_test.h>
#include <SofaMiscMapping/SquareDistanceMapping.h>


namespace sofa {
namespace {


/**  Test suite for SquareDistanceMapping.
 *
 * @author Matthieu Nesme
  */
template <typename SquareDistanceMapping>
struct SquareDistanceMappingTest : public Mapping_test<SquareDistanceMapping>
{
    typedef typename SquareDistanceMapping::In InDataTypes;
    typedef typename InDataTypes::VecCoord InVecCoord;
    typedef typename InDataTypes::Coord InCoord;

    typedef typename SquareDistanceMapping::Out OutDataTypes;
    typedef typename OutDataTypes::VecCoord OutVecCoord;
    typedef typename OutDataTypes::Coord OutCoord;


    bool test()
    {
        SquareDistanceMapping* map = static_cast<SquareDistanceMapping*>( this->mapping );
        map->f_computeDistance.setValue(true);
        map->d_geometricStiffness.setValue(1);

        component::topology::EdgeSetTopologyContainer::SPtr edges = modeling::addNew<component::topology::EdgeSetTopologyContainer>(this->root);
        edges->addEdge( 0, 1 );

        // parent positions
        InVecCoord incoord(2);
        InDataTypes::set( incoord[0], 0,0,0 );
        InDataTypes::set( incoord[1], 1,1,1 );

        // expected child positions
        OutVecCoord expectedoutcoord;
        expectedoutcoord.push_back( defaulttype::Vector1( 3 ) );

        return this->runTest( incoord, expectedoutcoord );
    }

};


// Define the list of types to instanciate.
using testing::Types;
typedef Types<
component::mapping::SquareDistanceMapping<defaulttype::Vec3Types,defaulttype::Vec1Types>
, component::mapping::SquareDistanceMapping<defaulttype::Rigid3Types,defaulttype::Vec1Types>
> DataTypes; // the types to instanciate.

// Test suite for all the instanciations
TYPED_TEST_CASE( SquareDistanceMappingTest, DataTypes );

// test case
TYPED_TEST( SquareDistanceMappingTest , test )
{
    ASSERT_TRUE(this->test());
}

} // namespace
} // namespace sofa