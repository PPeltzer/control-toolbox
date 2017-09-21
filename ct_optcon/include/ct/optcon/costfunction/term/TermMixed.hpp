/***********************************************************************************
Copyright (c) 2017, Michael Neunert, Markus Giftthaler, Markus Stäuble, Diego Pardo,
Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of ETH ZURICH nor the names of its contributors may be used
      to endorse or promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL ETH ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************************/


#ifndef CT_COSTFUNCTION_TERM_MIXED_HPP_
#define CT_COSTFUNCTION_TERM_MIXED_HPP_

#include "TermBase.hpp"

namespace ct {
namespace optcon {

/**
 * \ingroup CostFunction
 *
 * \brief A basic quadratic term of type \f$ J = u^T P x \f$
 *
 *	An example for using this term is given in \ref CostFunctionTest.cpp
 */
template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR = double, typename SCALAR2 = SCALAR>
class TermMixed : public TermBase<STATE_DIM, CONTROL_DIM, SCALAR, SCALAR2> {

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	CT_OPTCON_DEFINE_TERM_TYPES

	TermMixed();

	TermMixed(const control_state_matrix_t& P);

	TermMixed(const control_state_matrix_t& P, const core::StateVector<STATE_DIM, SCALAR>& x_ref, core::ControlVector<CONTROL_DIM, SCALAR>& u_ref);

	TermMixed(const TermMixed& arg);

	virtual ~TermMixed(){}
	
	TermMixed<STATE_DIM, CONTROL_DIM, SCALAR, SCALAR2>* clone () const override;

	void setWeights(const control_state_matrix_double_t& P);

	void setStateAndControlReference(const core::StateVector<STATE_DIM>& x_ref, const core::ControlVector<CONTROL_DIM>& u_ref);

	SCALAR2 evaluate(const Eigen::Matrix<SCALAR2, STATE_DIM, 1> &x, const Eigen::Matrix<SCALAR2, CONTROL_DIM, 1> &u, const SCALAR2& t) override
	{
	    Eigen::Matrix<SCALAR2, STATE_DIM, 1> xDiff = (x-x_ref_.template cast<SCALAR2>());
	    Eigen::Matrix<SCALAR2, CONTROL_DIM, 1> uDiff = (u-u_ref_.template cast<SCALAR2>());

	    return (uDiff.transpose() * P_.template cast<SCALAR2>() * xDiff)(0,0);		
	}	

	core::StateVector<STATE_DIM, SCALAR> stateDerivative(const core::StateVector<STATE_DIM, SCALAR> &x,
			const core::ControlVector<CONTROL_DIM, SCALAR> &u, const SCALAR& t) override;

	state_matrix_t stateSecondDerivative(const core::StateVector<STATE_DIM, SCALAR> &x,
			const core::ControlVector<CONTROL_DIM, SCALAR> &u, const SCALAR& t) override;
	
	core::ControlVector<CONTROL_DIM, SCALAR> controlDerivative(const core::StateVector<STATE_DIM, SCALAR> &x,
			const core::ControlVector<CONTROL_DIM, SCALAR> &u, const SCALAR& t) override;
	
	control_matrix_t controlSecondDerivative(const core::StateVector<STATE_DIM, SCALAR> &x,
			const core::ControlVector<CONTROL_DIM, SCALAR> &u, const SCALAR& t) override;

	control_state_matrix_t stateControlDerivative(const core::StateVector<STATE_DIM, SCALAR> &x,
			const core::ControlVector<CONTROL_DIM, SCALAR> &u, const SCALAR& t) override;
	
	virtual void loadConfigFile(const std::string& filename, const std::string& termName, bool verbose = false) override;

	void updateReferenceState (const Eigen::Matrix<SCALAR, STATE_DIM, 1>& newRefState) override{ x_ref_ = newRefState;}

protected:
	control_state_matrix_t P_;

	core::StateVector<STATE_DIM, SCALAR> x_ref_;
	core::ControlVector<CONTROL_DIM, SCALAR> u_ref_;

};

#include "implementation/TermMixed.hpp"

} // namespace optcon
} // namespace ct

#endif
