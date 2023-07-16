#ifndef AI_CLASS_H
#define AI_CLASS_H

#include "scene/3d/sprite_3d.h"
#include "scene/3d/physics_body_3d.h"
#include "modules/sprite_rotater/sprite_rotater.h"
#include "modules/ani_timer/ani_timer.h"

class AI : public CharacterBody3D
{
	GDCLASS(AI, CharacterBody3D);

	//private variables
	SpriteRotater rotater;
	Array id_target;
	Array target_signal;
	Array dictChanges;
	int row;
	int spriteAngle;
	int rd;
	int rc;
	float sightDist;
	float yRotation;
	float speed;
	bool startIdle;
	bool walking;
	String playerGroup;
    String scentGroup;
	Vector3 velocity;
	Vector3 oldVelocity;

	//private functions
	void idleMove(const Vector3& turnMove);
	bool playerInRange(const Vector3& vec);
	float playerDot(const Vector3& vec);
	
	protected:
		static void _bind_methods();
		void _notification(int p_what);
		
	public:
		AI();
		bool checkForPlayer3D(const Dictionary& result, Object* p_dir_timer);
		bool checkForPlayer(Object* p_sprite, const Dictionary& result, Object* p_dir_timer, Object* p_ani_timer);
		bool lookForPlayer(const Dictionary& result);
		Dictionary rayShot(const Vector3& vec1, const Vector3& vec2, const Array& vecExclude = Array());
		void setHorizontalVelocity(const Vector3& vel);
        Dictionary followScentTrail(const Array& vec3s);

		//getters and setters
		void setVelocity(const Vector3& vec);
		Vector3 getVelocity();
		void setRow(int newRow);
		int getRow();
		void setSpriteAngle(int newSpriteAngle);
		int getSpriteAngle();
		void setRandDistribution(int newRd);
		int getRandDistribution();
		void setRandChance(int newRc);
		int getRandChance();
		void setStartIdle(bool isStartIdle);
		bool getStartIdle();
		void setWalking(bool isWalking);
		bool getWalking();
		void setSightDist(float newSightDist);
		float getSightDist();
		void setSpeed(float customSpeed);
		float getSpeed();
		void setPlayerGroup(String group);
		String getPlayerGroup();
        void setScentGroup(String group);
		String getScentGroup();
		SpriteRotater* getRotater();
		float getYRotation();
		void setYRotation(float newYRotation);
		void setIdTarget(const Array& idTarget);
		Array getIdTarget();
		void setTargetSignal(const Array& targetSignal);
		Array getTargetSignal();
		void setDictChanges(const Array& newDictChanges);
		Array getDictChanges();
};

#endif //AI_CLASS_H
