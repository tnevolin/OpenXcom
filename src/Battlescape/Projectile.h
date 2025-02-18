#pragma once
/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <vector>
#include "Position.h"
#include "BattlescapeGame.h"

namespace OpenXcom
{

class BattleItem;
class SavedBattleGame;
class Surface;
class Tile;
class Mod;

struct	// Real Accuracy mod configuration
{
	int MinCap = 5;			// Minimum accuracy value
	int MaxCap = 95;		// Maximum accuracy value
	int AimBonus = 3;		// Additional bonus for aimed shot if total accuracy is 5% or less
	int KneelBonus = 2;		// Additional bonus for kneeling if total accuracy is 5% or less
	int aimedDivider = 4;
	int snapDivider = 3;	// Dividers are used for adding roll-based deviation
	int autoDivider = 3;
	int twoHandsBonus = 1;	// Less shots dispersion for one weapon in both hands
	int distanceDivider = 3;// Additional 1 voxel of deviation per this number of distance tiles

	double SizeMultiplier = 1.35; // Accuracy multiplier when targeting big units

	int suicideProtectionDistance = 50;	// Missing shot shouldn't land too close to a shooter
	int bonusDistanceMax = 10; // Improved accuracy distance - top threshold
	int bonusDistanceMin = 6; // Improved accuracy distance - bottom threshold
} AccuracyMod;

/**
 * A class that represents a projectile. Map is the owner of an instance of this class during its short life.
 * It calculates its own trajectory and then moves along this pre-calculated trajectory in voxel space.
 */
class Projectile
{
public:
	/// Offset of voxel path where item should be drop.
	static const int ItemDropVoxelOffset = -2;

	/// Get Position at offset from start from trajectory vector.
	static Position getPositionFromStart(const std::vector<Position>& trajectory, int pos);
	/// Get Position at offset from end from trajectory vector.
	static Position getPositionFromEnd(const std::vector<Position>& trajectory, int pos);

private:
	Mod *_mod;
	SavedBattleGame *_save;
	BattleAction _action;
	Position _origin, _targetVoxel;
	std::vector<Position> _trajectory;
	size_t _position;
	float _distance;
	int _speed;
	int _bulletSprite;
	bool _reversed;
	int _vaporColor, _vaporDensity, _vaporProbability;
	void applyAccuracy(Position origin, Position *target, double accuracy, bool keepRange, bool extendLine);
public:
	/// Creates a new Projectile.
	Projectile(Mod *mod, SavedBattleGame *save, BattleAction action, Position origin, Position target, BattleItem *ammo);
	/// Cleans up the Projectile.
	~Projectile();
	/// Calculates the trajectory for a straight path.
	int calculateTrajectory(double accuracy);
	int calculateTrajectory(double accuracy, const Position& originVoxel, bool excludeUnit = true);
	/// Calculates the trajectory for a curved path.
	int calculateThrow(double accuracy);
	/// Moves the projectile one step in its trajectory.
	bool move();
	/// Gets the current position in voxel space.
	Position getPosition(int offset = 0) const;
	/// Gets the two last position in voxel space.
	LastPositions getLastPositions(int offset = 0) const { return LastPositions(getPosition(offset), getPosition(offset + ItemDropVoxelOffset)); }
	/// Gets a particle from the particle array.
	int getParticle(int i) const;
	/// Gets the item.
	BattleItem *getItem() const;
	/// Skips the bullet flight.
	void skipTrajectory();
	/// Gets the Position of origin for the projectile.
	Position getOrigin() const;
	/// Gets the targetted tile for the projectile.
	Position getTarget() const;
	/// Gets the distance that projectile traveled.
	float getDistance() const;
	/// Is this projectile being drawn back-to-front or front-to-back?
	bool isReversed() const;
	/// adds a cloud of particles at the projectile's location
	void addVaporCloud();
};

}
