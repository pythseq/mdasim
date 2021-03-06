/*
Copyright 2012- Zeinab Taghavi (ztaghavi@wayne.edu)

    This file is part of MDAsim.

    MDAsim is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    MDAsim is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MDAsim; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/***************************************************************************
 * Title:          mdasim.h
 * Author:         Zeinab Taghavi
 * Created:        2012
 * Last modified:  5/27/2014
 *
 * Copyright (c) 2012- Zeinab Taghavi
 * All Rights Reserved
 * See file LICENSE for details.
 ***************************************************************************/
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <limits.h>
#include <utility>

#include "commonMDA.h"
#include "getopt.h"

#define CoverageToPrimerConst 1000

using namespace std;

typedef int64_t FragmentID;
typedef int64_t Coord;
typedef char Base;
typedef int64_t Coverage;
typedef basic_string<Base> DNAType;

#define MaxFragmentID LONG_MAX

/**
 * Positions store information about a single position on a fragment like
 * the fragment ID, the position on the fragment and the reference position
 * on the original sequence
 */
struct Position
{
        // Reference to the ID of the fragment where this position has been copied from
        // IF this position is still connected to that fragment
        // otherwise it's 0 (unoccupied)
        // it is fragmentNo+1
        FragmentID fragmentNo1;
        // Position on the fragment
        Coord pos;
        // Position on the original sequence, added in #2.0
        // negative if Position is located on negative strand
        Coord original;

        // Comparator. A Position is smaller than another,
        // if it's on a fragment created earlier during the amplifiction
        // or if both are on the same fragment, if it's position on the fragment is smaller
  bool const operator <(const Position rhs) const
  {
    if (fragmentNo1 < rhs.fragmentNo1)
      return true;
    else if (fragmentNo1 > rhs.fragmentNo1)
      return false;
    else
      return (pos < rhs.pos);
        }
};

/**
 * FragmentBase consists of the base and Position
 */
struct FragmentBase {
  Base base;
  Position occupancy;
};

typedef basic_string<FragmentBase> FragmentDNAType;
struct Fragment {
  FragmentDNAType dna;
};

typedef vector<Fragment> FragmentList;
typedef vector<DNAType> ReadsList;
typedef DNAType Primer;
typedef vector<Primer> PrimerList;
struct PrimerPosition {
  vector<Position> list;
  Primer ReverseComplement;
  Coverage currentNo;
  Coverage availalePositionsNo;
  Coverage availableFreePositionsNo;
};

/* reverse complement Primer list and its availability */
typedef map<Primer, PrimerPosition> PrimerPositionList;

/**
 * @brief The Phi29 struct wraps the state of a single copy process
 */
struct Phi29 {
    FragmentID fragmentNo1;
  Coord expectedLength;
  Position currentPosition;
};

typedef list<Phi29> Phi29List;
typedef vector<Coverage> AlignmentList;

typedef set<Position> PositionSet;

/**
 * @brief The FreePrimerPosition struct
 */
struct FreePrimerPosition {
  PositionSet set;
  Coverage currentNo;
};

typedef map<Primer, FreePrimerPosition> FreePrimerPositionSets;
typedef pair<Primer, FreePrimerPosition> PrimerPositionPair;

struct AttachmentMerit {
  Primer	primer;
  Coverage coverage;
  Coverage phi29AttachmentNum;
};
/* a predicate for sorting implemented as function */
inline bool AttachmentCompare(AttachmentMerit aM1, AttachmentMerit aM2) { return (aM1.coverage > aM2.coverage);}

template <class T>
inline T rounddiv(T a1, T a2)
{
  if (a2 == 0)
    exitMsg((char *)" Error in rounddiv. a2 is zero.", INTERNAL_WOW_ERROR);
  T b = a1 / a2;
  if ((a1 % a2) > (a2 / 2))
    b ++;
  return b;
}

template <class T>
inline T ceildiv(T a1, T a2)
{
  if (a2 == 0)
    exitMsg((char *)" Error in ceildiv. a2 is zero.", INTERNAL_WOW_ERROR);
  T b = a1 / a2;
  if ((a1 % a2) > 0)
    b ++;
  return b;
}
