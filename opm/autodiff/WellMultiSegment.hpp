/*
  Copyright 2015 SINTEF ICT, Applied Mathematics.

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

#ifndef OPM_WELLMULTISEGMENT_HEADER_INCLUDED
#define OPM_WELLMULTISEGMENT_HEADER_INCLUDED


#include <opm/core/wells.h>
#include <opm/core/well_controls.h>
#include <opm/core/simulator/WellState.hpp>
#include <opm/core/utility/ErrorMacros.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/ScheduleEnums.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/SegmentSet.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Well.hpp>
#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <map>
#include <algorithm>
#include <array>

namespace Opm
{

    class WellMultiSegment
    {
    public:


        size_t numberOfPerforations() const;
        size_t numberOfSegment() const;

        const std::vector<double>& wellIndex() const;
        const std::vector<double>& perfDepth() const;
        const std::vector<int>& wellCell() const;
        const std::vector<int>& outletSegment() const;
        const std::vector<double>& segmentLength() const;
        const std::vector<double>& segmentDepth() const;
        const std::vector<double>& segmentCrossArea() const;
        const std::vector<double>& segmentRoughness() const;
        const std::vector<double>& segmentVolume() const;
        const std::vector<std::vector<int>>& segmentPerforatioins() const;

    private:
        // for the moment, we use the information from wells.
        // TODO: remove the dependency on wells from opm-core.
        void init(WellConstPtr well, size_t time_step, const Wells* wells);

    private:
        // name of the well
        std::string m_well_name;
        // flag to indicate if this well is a
        // multi-segmented well
        bool m_is_multi_segment_;
        // components of the pressure drop to be included
        WellSegment::CompPresureDropEnum m_comp_pressure_drop_;
        // multi-phase flow model
        WellSegment::MultiPhaseModelEnum m_multiphase_model_;
        // number of perforation for this well
        size_t m_number_of_perforations_;
        // number of segments for this well
        size_t m_number_of_segments_;
        // well index for each completion
        std::vector<double> m_well_index_;
        // depth for each completion
        std::vector<double> m_perf_depth_;
        // well cell for each completion
        std::vector<int> m_well_cell_;
        // how to organize the segment structure here?
        // indicate the outlet segment for each segment
        // maybe here we can use the location in the vector
        // at the moment, we still use the ID number
        // then a mapping from the ID number to the actual location will be required
        std::vector<int> m_outlet_segment_;
        std::map<int, int> m_number_to_location_;
        // has not decided to use the absolute length from the well head
        // or the length of this single segment
        // using the absolute length first
        std::vector<double> m_segment_length_;
        // the depth of the segmnet node
        std::vector<double> m_segment_depth_;
        // the internal diameter of the segment
        std::vector<double> m_segment_internal_diameter_;
        // the roughness of the segment
        std::vector<double> m_segment_roughness_;
        // the cross area of the segment
        std::vector<double> m_segment_cross_area_;
        // the volume of the segment
        std::vector<double> m_segment_volume_;
        // the completions that is related to each segment
        // the completions's ids are their location in the vector m_well_index_
        // m_well_cell_
        std::vector<std::vector<int>> m_segment_perforations_;
    };

    typedef std::shared_ptr<WellMultiSegment> WellMutliSegmentPtr;
    typedef std::shared_ptr<const WellMultiSegment> WellMutliSegmentConstPtr;

} // namespace Opm


#endif // OPM_WELLMULTISEGMENT_HEADER_INCLUDE
