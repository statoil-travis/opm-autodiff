/*
  Copyright 2015 SINTEF ICT, Applied Mathematics.
  Copyright 2015 IRIS AS
  Copyright 2015 Dr. Blatt - HPC-Simulation-Software & Services
  Copyright 2015 NTNU
  Copyright 2015 Statoil AS
  Copyright 2015 IRIS AS

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_NEWTONITERATIONBLACKOILINTERLEAVED_HEADER_INCLUDED
#define OPM_NEWTONITERATIONBLACKOILINTERLEAVED_HEADER_INCLUDED

#include <opm/autodiff/NewtonIterationBlackoilInterface.hpp>
#include <opm/core/utility/parameters/ParameterGroup.hpp>

#include <array>
#include <memory>

namespace Opm
{
    /// This class carries all parameters for the NewtonIterationBlackoilInterleaved class
    struct NewtonIterationBlackoilInterleavedParameters
    {
        double linear_solver_reduction_;
        int    linear_solver_maxiter_;
        int    linear_solver_restart_;
        int    linear_solver_verbosity_;
        bool   newton_use_gmres_;

        NewtonIterationBlackoilInterleavedParameters() { reset(); }
        // read values from parameter class
        NewtonIterationBlackoilInterleavedParameters( const parameter::ParameterGroup& param )
        {
            // set default parameters
            reset();

            // read parameters (using previsouly set default values)
            newton_use_gmres_        = param.getDefault("newton_use_gmres", newton_use_gmres_ );
            linear_solver_reduction_ = param.getDefault("linear_solver_reduction", linear_solver_reduction_ );
            linear_solver_maxiter_   = param.getDefault("linear_solver_maxiter", linear_solver_maxiter_);
            linear_solver_restart_   = param.getDefault("linear_solver_restart", linear_solver_restart_);
            linear_solver_verbosity_ = param.getDefault("linear_solver_verbosity", linear_solver_verbosity_);
        }

        // set default values
        void reset()
        {
            newton_use_gmres_        = false;
            linear_solver_reduction_ = 1e-2;
            linear_solver_maxiter_   = 50;
            linear_solver_restart_   = 40;
            linear_solver_verbosity_ = 0;
        }
    };


    /// This class solves the fully implicit black-oil system by
    /// solving the reduced system (after eliminating well variables)
    /// as a block-structured matrix (one block for all cell variables).
    class NewtonIterationBlackoilInterleaved : public NewtonIterationBlackoilInterface
    {
    public:

        /// Construct a system solver.
        /// \param[in] param   parameters controlling the behaviour of the linear solvers
        /// \param[in] parallelInformation In the case of a parallel run
        ///                                with dune-istl the information about the parallelization.
        NewtonIterationBlackoilInterleaved(const parameter::ParameterGroup& param,
                                           const boost::any& parallelInformation=boost::any());

        /// Solve the system of linear equations Ax = b, with A being the
        /// combined derivative matrix of the residual and b
        /// being the residual itself.
        /// \param[in] residual   residual object containing A and b.
        /// \return               the solution x
        virtual SolutionVector computeNewtonIncrement(const LinearisedBlackoilResidual& residual) const;

        /// \copydoc NewtonIterationBlackoilInterface::iterations
        virtual int iterations () const { return iterations_; }

        /// \copydoc NewtonIterationBlackoilInterface::parallelInformation
        virtual const boost::any& parallelInformation() const;

    private:
        // max number of equations supported, increase if necessary
        static const int maxNumberEquations_ = 6 ;

        mutable std::array< std::unique_ptr< NewtonIterationBlackoilInterface >, maxNumberEquations_+1 > newtonIncrement_;
        NewtonIterationBlackoilInterleavedParameters parameters_;
        boost::any parallelInformation_;
        mutable int iterations_;
    };

} // namespace Opm


#endif // OPM_NEWTONITERATIONBLACKOILINTERLEAVED_HEADER_INCLUDED
