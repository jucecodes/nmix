/*
 *  Copyright 2016 Antonio Lassandro
 *
 *  nmix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nmix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NMIX_COMMANDIDS_H
#define NMIX_COMMANDIDS_H

namespace nmix
{

enum Operations
{
    None = 0,
    Escape,
    
    SelectAll,
    InverseSelect,
    DeselectAll,
    
    AddNode,
    RemoveNode,
    
    NudgeSelection,
    LockSelection,
    
    AdjustX,
    AdjustY,
    AdjustVolume,
    AdjustBalance,
    
};

}

#endif  // NMIX_COMMANDIDS_H