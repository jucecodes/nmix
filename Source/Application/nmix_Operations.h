/*
 *  Copyright 2016 - 2017 Antonio Lassandro
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

enum Operation
{
    None = 0,
    Escape,

// Singleton Operations

    // General

    CycleEditMode,

    //Nodes

    SelectAll,
    InvertSelection,
    DeselectAll,
    
    AddNode,
    RemoveNode,

    LockSelection,
    UnlockSelection,

    NudgeSelection,
    CentreSelection,

    // Anchor

    SetAnchor,
    SnapAnchor,
    ResetAnchor,
    CentreAnchor,

// Stateful Operations

    PositionSelectionX,
    PositionSelectionY,
    PositionSelectionDistance,
    PositionSelectionAzimuth,

};

enum EditMode
{

    Parallel = 0,
    Inverse,

};

}

#endif  // NMIX_COMMANDIDS_H
