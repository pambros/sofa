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

#ifndef SOFA_COMPONENT_MASS_AffineMass_H
#define SOFA_COMPONENT_MASS_AffineMass_H

#include <sofa/core/behavior/Mass.h>
#include <SofaEigen2Solver/EigenSparseMatrix.h>

namespace sofa
{

namespace component
{

namespace mass
{

/**
*  Mass for affine frames stored as a compressed row sparse matrix
*/

template <class DataTypes>
class AffineMass : public core::behavior::Mass< DataTypes >
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(AffineMass,DataTypes), SOFA_TEMPLATE( core::behavior::Mass, DataTypes ) );

    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef SReal Real;
    typedef defaulttype::Vec<3,Real> Vec3;
    typedef core::objectmodel::Data<VecCoord> DataVecCoord;
    typedef core::objectmodel::Data<VecDeriv> DataVecDeriv;

    typedef component::linearsolver::EigenSparseMatrix<DataTypes,DataTypes> MassMatrix;

protected:

    AffineMass()
        : core::behavior::Mass<DataTypes>()
        , d_massMatrix( initData(&d_massMatrix, "massMatrix", "Mass Matrix") )
    {
    }

    virtual ~AffineMass()
    {
    }

public:

    virtual void init() override
    {
        core::behavior::Mass<DataTypes>::init();
    }

    virtual void bwdInit() override
    {
        // if the mass matrix is not given manually -> set to identity
        if( d_massMatrix.getValue().rows() != d_massMatrix.getValue().cols() || (unsigned)d_massMatrix.getValue().rows()!=this->mstate->getMatrixSize() )
        {
            MassMatrix& massMatrix = *d_massMatrix.beginWriteOnly();
            massMatrix.resize(this->mstate->getMatrixSize(),this->mstate->getMatrixSize());
            massMatrix.setIdentity();
            massMatrix.compress();
            d_massMatrix.endEdit();
        }
    }

    void addMDx(const core::MechanicalParams* /*mparams*/, DataVecDeriv& f, const DataVecDeriv& dx, SReal factor) override
    {
        if( factor == 1.0 ) d_massMatrix.getValue().addMult( f, dx );
        else d_massMatrix.getValue().addMult( f, dx, factor );
    }

    void accFromF(const core::MechanicalParams* /*mparams*/, DataVecDeriv& /*a*/, const DataVecDeriv& /*f*/) override
    {
        serr<<"accFromF not yet implemented (the matrix inversion is needed)"<<sendl;
    }

    void addForce(const core::MechanicalParams* /*mparams*/, DataVecDeriv& f, const DataVecCoord& /*x*/, const DataVecDeriv& /*v*/) override
    {
        //if gravity was added separately (in solver's "solve" method), then nothing to do here
        if(this->m_separateGravity.getValue()) return;

        VecDeriv& _f = *f.beginEdit();

        // gravity
        Vec3 g ( this->getContext()->getGravity() );
        if(g[0]==0 && g[1]==0 && g[2]==0) return;

        //TODO optimize this!!!
        VecDeriv gravities(_f.size());
        for(size_t i=0 ; i<_f.size() ; ++i )
            DataTypes::set( gravities[i], g[0], g[1], g[2] );
        d_massMatrix.getValue().addMult( _f, gravities );

        f.endEdit();
    }

    SReal getKineticEnergy(const core::MechanicalParams* /*mparams*/, const DataVecDeriv& v) const override
    {
        const VecDeriv& _v = v.getValue();
        SReal e = 0;
        VecDeriv Mv;
        d_massMatrix.getValue().mult( Mv, _v );
        for( unsigned int i=0 ; i<_v.size() ; i++ ) e += _v[i] * Mv[i];
        return e/2;
    }

    SReal getPotentialEnergy(const core::MechanicalParams* mparams, const DataVecCoord& x) const override
    {
        serr<<SOFA_CLASS_METHOD<<"not implemented!\n";
        return core::behavior::Mass< DataTypes >::getPotentialEnergy( mparams, x );
    }

    void addGravityToV(const core::MechanicalParams* mparams, DataVecDeriv& d_v) override
    {
        if(mparams)
        {
            VecDeriv& v = *d_v.beginEdit();

            // gravity
            Vec3 g ( this->getContext()->getGravity() * sofa::core::mechanicalparams::dt(mparams) );
            Deriv theGravity;
            DataTypes::set ( theGravity, g[0], g[1], g[2]);
            Deriv hg = theGravity * sofa::core::mechanicalparams::dt(mparams);

            // add weight force
            for (unsigned int i=0; i<v.size(); i++)
            {
                v[i] += hg;
            }
            d_v.endEdit();
        }
    }

    void addMToMatrix(const core::MechanicalParams *mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix) override
    {
        sofa::core::behavior::MultiMatrixAccessor::MatrixRef r = matrix->getMatrix(this->mstate);
        Real mFactor = (Real)sofa::core::mechanicalparams::mFactorIncludingRayleighDamping(mparams, this->rayleighMass.getValue());
        d_massMatrix.getValue().addToBaseMatrix( r.matrix, mFactor, r.offset );
    }

    Data<MassMatrix> d_massMatrix; ///< mass matrix
};



} // namespace mass
} // namespace component
} // namespace sofa

#endif // __AffineMass_H
