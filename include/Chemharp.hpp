/*
* Chemharp, an efficient IO library for chemistry file formats
* Copyright (C) 2015 Guillaume Fraux
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/
*/

/*! @file Chemharp.hpp
* Chemharp main C++ API header.
*
* This file includes the public API headers for Chemharp, and should be the only
* one included by client applications.
*/

#ifndef CHEMHARP_HPP
#define CHEMHARP_HPP

// This will be used as a protection to prevent internal macro from being exported
#define CHEMHARP_PUBLIC

#include "chemharp/config.hpp"

#include "chemharp/Logger.hpp"
#include "chemharp/Error.hpp"

#include "chemharp/Atom.hpp"
#include "chemharp/Topology.hpp"
#include "chemharp/Frame.hpp"
#include "chemharp/UnitCell.hpp"
#include "chemharp/Trajectory.hpp"

#undef CHEMHARP_PUBLIC

#endif // HARP_HPP