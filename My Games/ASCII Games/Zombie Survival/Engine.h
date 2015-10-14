#ifndef ENGINE_H
#define ENGINE_H

#define mapHeight 30
#define mapWidth 40

bool isPassable(int y, int x);
int testForActor(int y, int x);
bool isPassableBullet(int y, int x);
void bulletHit(int actorId, int damage);
bool addSpecialBulletToListSpecial(int direction, int y, int x, int range, int speed, int damage, bool rocket, bool granade, bool rocketExplosion, bool frag, bool random, bool mine, bool granadeGranade, bool granadeGranadeGranade, bool potato, bool piercing);

extern int mapArray[mapHeight][mapWidth];

extern int cursorY;
extern int cursorX;

extern int playerMovementSpeed;

extern int playerHealth;
extern int playerMaxHealth;
extern int playerMaxStamina;
extern int playerStaminaRegen;
extern int playerAmmo;
extern int playerBulletSpeed;
extern int playerDamage;
extern int playerBulletRange;
extern int playerFiringSpeed;
extern int playerClip;
extern int playerClipLimit;
extern int playerReloadingRate;
extern int playerReloadingModifier;
extern bool playerSpecialWeaponTypes[10];
extern char playerWeaponName[30];
extern int playerCurrentWeapon[2];
extern int playerBandages;
extern int playerMeleeDamage;
extern int playerStaminaPills;

extern int playerMoney;
extern int killMoney;
extern int playerXP;
extern int playerXPNeeded;
extern int playerXPGet;
extern int playerSkillPoints;
extern bool lastTiersUnlocked;

extern float roundZombiesFloat;
extern int zombieSpawnRate;

extern int playerZombiesKilled;
extern int playerTotalMoneyEarned;
extern int playerAmountBought;
extern int playerTimesHit;

#endif // ENGINE_H
