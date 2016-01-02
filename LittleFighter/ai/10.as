int pre_hp=500;
int elosshp=0;
int recover = 0;

int victory=0;
int start =0;

int input_x = 0;
int input_z = 0;

//int enid = -1;

array<int> AttSeq = {30,10,6,2,8,1,4,6,5,8};
int SeqLen = AttSeq.length();
int countAtt = 2;
int clrVal = 0;
int lasthurt =0;


int pre_mp = 300;
int lossmp = 0;

void cleanButton(){
	right(0,0);
	left(0,0);
	up(0,0);
	down(0,0);
	J(0,0);
	A(0,0);
	D(0,0);
}

int getfitness(int lossmp, int enhurt){
	int fit = 0;
	fit = enhurt - lossmp;
	return fit;
}

void LEFT(){
	left();
}

void RIGHT(){
	right();
}
void UP(){
	up();
}
void DOWM(){
	down();
}
void ATT(){
	A();
}
void DEF(){
	D();
}
void JUM(){
	J();
}

void id(){
	input_x = AttSeq[0];
	input_z = AttSeq[1];
	cleanButton();
	if(clrVal == 1) {
		start = 0;
		countAtt = -1;
		pre_hp=500;
		elosshp=0;
		recover = 0;
		pre_mp = 300;
		lossmp = 0;
	}
	float seed = rand(100);
	seed /= 100;
	clr();
	person me = person(true);
	person@ competitor = null;
	//*
	//print("me:" + " hp=" + self.hp + ", mp=" + self.mp + " , loss hp="+ losshp + " , KilledEnemies="+KilledEnemies +"\n");
	//me.toScreen();
	//print("\n");
	array<person> enemies;
	for(int i=0; i<400; i++){
		//剔除自己
		if(loadTarget(i) != -1 ){
			if(target.num != self.num ){
				if(target.type == 0){
					float dx = (self.facing ? self.x - target.x : target.x - self.x);
					//敵方
					enemies.insertLast( person(false));
					if( @competitor == null || (competitor.state == 14 && target.state != 14) || (competitor.absDx() > abs(dx) && target.state != 14)){
						@competitor = enemies[enemies.length()-1];
						break;
					}
				}
			}	
		}
	}
	//結束load敵人資料
	if(target.hp >= pre_hp+10){
		recover = 1;
	}
	if (target.hp<=pre_hp)	{
		elosshp += pre_hp-target.hp;
		pre_hp = target.hp;
	}
	if(self.mp <= pre_mp-10){
		lossmp += pre_mp-self.mp;
		pre_mp = self.mp;
	}
	//cleanButton();
	//清除所有已按下的按鍵
	fuzzyValue x_near("near",-1,0,50,90);
	fuzzyValue x_mid_near("mid_near",50,80,100,140);
	fuzzyValue x_mid_far("mid_far",100, 150, 300,350);
	fuzzyValue x_far("far",320, 400, 3000, 5000);
	fuzzySet x_dis("x distance");
	x_dis.push(x_near);
	x_dis.push(x_mid_near);
	x_dis.push(x_mid_far);
	x_dis.push(x_far);
	
	fuzzyValue z_near("near",-1,0,20,40);
	fuzzyValue z_mid("mid",25,40,60,80);
	fuzzyValue z_far("far",70, 85, 3000, 5000);
	fuzzySet z_dis("z distance");
	z_dis.push(z_near);
	z_dis.push(z_mid);
	z_dis.push(z_far);
	
	fuzzyValue y_near("near",-1,0,70,100);
	fuzzyValue y_far("far",80, 120, 3000, 5000);
	fuzzySet y_dis("y distance");
	y_dis.push(y_near);
	y_dis.push(y_far);
	//print("self = "+self.state+"\n");
	//print("att"+countAtt+"\n");
	if( (@competitor == null || competitor.state == 14 ||recover == 1) && start == 1 && countAtt >5){
		//print("game over\n");
		//print("start = "+start+"\n");
		//print(competitor.state+"\n");
		//right(1,1);
		//else 
		//exit(1);
		cleanButton();
		//print("lossmp : "+lossmp+","+"Hurt : "+elosshp+"\n");
		int Fitness = getfitness(lossmp, elosshp);
		print(Fitness);
		return ;
	}

	float[] dx = x_dis.fuzzify(competitor.absDx());
	float[] dy = y_dis.fuzzify(competitor.absDy());
	float[] dz = z_dis.fuzzify(competitor.absDz());

	//手上有東西 馬上丟
	if( self.weapon_type != 0 ){
		J();
		if (competitor.dx > 0)
			right(1,1);
		else
			left(1,1);
		A();
		return;
	}
	//print("target : "+target.x+","+target.type+"\n");
	if(abs(target.x - self.x)<input_x && abs(target.z - self.z)<input_z )
		start = 1;
	
	//開始按方向鍵
	if(start == 0){
		cleanButton();
		if(target.x - self.x > input_x  && 1 > seed){
			right(1,1);
			return;
		}
		else if(self.x -target.x > input_x && 1 > seed){

			left(1,1);
			return ;
		}
		if(target.z - self.z > input_z && 1 > seed){
			down(1,1);
			return;
		}
		else if(self.z - target.z > input_z && 1 > seed){
			up(1,1);
			return;
		}
	}
	//開始按招式
	//cleanButton();
	if(countAtt >= AttSeq.length()) {
		//print("Attack over\n");
		int Fitness = getfitness(lossmp, elosshp);
		print(Fitness);
		return;
	}
	if(AttSeq[countAtt] == 9){
		countAtt++;
		return;
	}	
	if(AttSeq[countAtt] == 1){
		cleanButton();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 2){
		UP();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 3){
		DOWM();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 4){//back
		if(target.x - self.x > 0) LEFT();
		else RIGHT();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 5){//forward
		if(target.x - self.x >0) RIGHT();
		else LEFT();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 6){
		DEF();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 7){
		JUM();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 8){
		ATT();
		countAtt++;
		return;
	}
}

/* 相對於自己的座標方位
 *           /----/----/
 *          /   z>0   /|
 *         /    /    / |
 *        /----/----/  /
 *       /    /    /| /|
 *      /   z<0   / |/ |
 * y>0 /----/----/  /  /
 *     |    |    | /| /
 *     |    |    |/ |/
 * x<0 |---------/  /   x>0
 *     |    |    | /
 *     |    |    |/
 * y<0 |---------/
 */

class person{
	int blink;		//是否閃爍
	int id;			//物件識別碼
	int team;		//隊伍
	int frame;		//在第幾影格
	int holding;	//持有何種武器，-1表示空手，否則為所持武器type
	int armor;		//目標盔甲型態，0無盔甲，1一般盔甲，2魔王盔甲(防冰火)
	int state;		//目標狀態
	int facing;		//目標面向，-1 向左，1向右
	int hostage;    //隊友數量
	float x;		//目標位置
	float y;
	float z;
	float dx;		//以自己為原點的目標位置
	float dy;
	float dz;
	float vx;		//目標移動向量
	float vy;
	float vz;
	//array<person> teammates;
	
	person(){
		dx=0;dy=0;dz=0;
	}
	
	//d=true，取自己的資料建構
	//d=false，取目標的資料建構
	person(bool d){
		if(d){
			blink = self.blink;
			id = self.id;
			team = self.team;
			frame = self.frame;
			if(id == 52) armor = 2;
			else if(id == 37 || id == 6) armor = 1;
			else armor = 0;
			state = self.state;
			facing = self.facing ? -1 : 1;
			x = self.x;
			y = self.y;
			z = self.z;
			dx = 0;
			dy = 0;
			dz = 0;
			vx = self.x_velocity;
			vy = -self.y_velocity;
			vz = -self.z_velocity;
			holding = (self.weapon_held == -1 ? -1 : self.weapon_type);
		}
		else{
			blink = target.blink;
			id = target.id;
			team = target.team;
			frame = target.frame;
			if(id == 52) armor = 2;
			else if(id == 37 || id == 6) armor = 1;
			else armor = 0;
			state = target.state;
			facing = target.facing ? -1 : 1;
			x = target.x;
			y = target.y;
			z = target.z;
			dx = target.x - self.x;
			dy = self.y - target.y;
			dz = self.z - target.z;
			vx = target.x_velocity;
			vy = -target.y_velocity;
			vz = -target.z_velocity;
			holding = (target.weapon_held == -1 ? -1 : target.weapon_type);
		}
	}
	person(float rx,float ry, float rz){
		dx=rx;dy=ry;dz=rz;
	}
	
	float absDx(){
		return abs(dx);
	}
	
	float absDy(){
		return abs(dy);
	}
	
	float absDz(){
		return abs(dz);
	}
	
	float nextDx(){
		return dx + vx;
	}
	
	float nextDy(){
		return dy + vy;
	}
	
	float nextDz(){
		return dz + vz;
	}
	
	void toScreen(){
		print("id=" + id + ", team=" + team +  "\n");
		print("pos=(" + x + ", " + y + ", " + z + ")\n");
		print("dpos=(" + dx + ", " + dy + ", " + dz + ")\n");
		print("vel=(" + vx + ", " + vy + ", " + vz + ")\n");
		print("frame=" + frame + ", state=" + state + ", facing=" + facing + ", hlding=" + holding + "\n");
	}
	
	bool isAttacking(){
		if(state == 3) return true;
		else if(state == 301) return true;		//鬼哭斬
		else if(state == 100) return true;		//落雷霸
		else if(state == 19) return true;		//烈火焚身
		else return false;
	}
	
	bool isDefending(){
		return state == 7;
	}
	
	bool isBrokenDenfending(){
		return state == 8;
	}
	
	//bool isHoldingWeapon(){}
	
	bool isLying(){
		return state == 14;
	}
	
	bool isDrinking(){
		return state == 17;
	}
	
	bool isSkilling(){
		return frame > 234;
	}
	
	bool isFalling(){
		return state == 12;
	}
	
	bool isStanding(){
		return state == 0;
	}
	
	bool isWalking(){
		return state == 1;
	}
	
	bool isRunning(){
		return state == 2;
	}
	
	bool isDashing(){
		return state == 5;
	}
	
	bool isJumping(){
		return state == 4;
	}
	
	bool isRolling(){
		return state == 6;
	}
	
	bool isFliping(){
		return (frame==100 || frame==101 || frame==108 || frame==109);
	}
	
	bool isObjing(){
		return (frame > 11 && frame < 20);
	}
	
	bool isFainting(){
		return state == 16;
	}
	
	bool isCatching(){
		return state == 9;
	}
	
	bool isCaught(){
		return state == 10;
	}
	
	bool isFreezing(){
		return frame == 200;
	}
	
	bool isFrozen(){
		return state == 13;
	}
	
	bool isOnFire(){
		return state == 18;
	}
	
	bool isOnGround(){
		if(self.y == 0) return dy==0;
		else return dy!=0;
	}
};
class oobject{
	int id;			//物件識別碼
	int type;		//物件類別
	int team;		//隊伍
	int frame;		//在第幾影格
	int state;		//目標狀態
	float x;		//目標位置
	float y;
	float z;
	float dx;		//以自己為原點的目標位置
	float dy;
	float dz;
	float vx;		//目標移動向量
	float vy;
	float vz;
	
	oobject(){
		id = target.id;
		type = target.type;
		team = target.team;
		frame = target.frame;
		state = target.state;
		x = target.x;
		y = target.y;
		z = target.z;
		dx = target.x - self.x;
		dy = self.y - target.y;
		dz = self.z - target.z;
		vx = target.x_velocity;
		vy = target.y_velocity;
		vz = target.z_velocity;
	}
	
	float absDx(){
		return abs(dx);
	}
	
	float absDy(){
		return abs(dy);
	}
	
	float absDz(){
		return abs(dz);
	}
	
	void toScreen(){
		print("id=" + id + ", team=" + team + ", type=" + type + "\n");
		print("pos=(" + x + ", " + y + ", " + z + ")\n");
		print("dpos=(" + dx + ", " + dy + ", " + dz + ")\n");
		print("vel=(" + vx + ", " + vy + ", " + vz + ")\n");
		print("frame=" + frame + ", state=" + state + "\n");
	}
	
	bool isDrink(){
		return type == 6;
	}
	
	bool isCriminal(){
		return type == 5;
	}
	
	bool isChee(){
		return type == 3;
	}
	
	bool isWeapon(){
		return type == 1;
	}
	
	bool isHeavy(){
		return type == 2;
	}
	
	bool isBall(){
		return type == 4;
	}
	
	bool isLying(){
		if(!isChee() && (isHeavy() ? state == 2004 : state == 1004)) return true;
		else return false;
	}
	
	bool isFalling(){
		if(!isChee() && (isHeavy() ? state == 2000 : state == 1000)) return true;
		else return false;
	}
	
	bool isOnHand(){
		if(isChee()) return false;
		else if(isHeavy()){
			if(state == 2001) return true;
			else return false;
		}
		else if(state == 1001) return true;
		else return false;
	}
	
	//the same as isThrowing
	bool isFlying(){
		if(isChee()){
			if(state == 3000) return true;
			else return false;
		}
		else if(isHeavy()){
			if(state == 2000) return true;
			else return false;
		}
		else if(state == 1002) return true;
		else return false;
	}
	
	bool isChasing(){
		if(isFlying()){
			if(id == 215) return true;						//Dennis
			else if(id == 214) return true;					//John
			else if(id == 219 || id == 220) return true;	//Jan
			else if(id == 221 || id == 222) return true;	//Firzen
			else if(id == 225) return true;					//Julian
			else if(id == 228) return true;					//Bat
			else return false;
		}
		else return false;
	}
	
	bool isCure(){
		if(id == 200 && frame >= 50 && frame < 55) return true;	//John
		else if(id == 220 && isFlying()) return true;			//Jan
		else return false;
	}
	
	bool isWall(){	//John ForceField
		if(id == 200 && frame >= 60 && frame <= 65) return true;
		else return false;
	}
	
	bool isPassable(){
		if(isHeavy() && isLying()) return false;			//heavy Object
		else if(id == 212 && state == 3000) return false;	//Freeze colume
		else return true;
	}
	
	bool isPickable(){
		if(!isChee() && isLying())return true;
		else return false;
	}
	
	bool isHitBreakable(){
		if(!isChee()) return true;
		else if(state == 3005 || state == 3006) return false;
		else if(id == 211) return false;					//Fire
		else if(isWall()) return false;						//John ForceField
		else if(id == 212 && frame > 100) return false;		//Freeze whirlwind
		else if(id == 229) return false;					//Julian col & exp & Bomb
		else return true;
	}
	
	bool isWind(){
		if(state == 3006) return true;
		else return false;
	}
	
	bool isBreakable(){
		if(id == 211) return false;							//Fire
		else if(id == 212 && frame > 100) return false;		//Freeze whirlwind
		else if(id == 229 && frame > 5) return false;		//Julian col & exp
		else return true;
	}
	
	bool isDefendable(){
		if(id == 212 && frame >= 150) return false;			//Freeze whirlwind
		else if(id == 229 && frame < 100) return false;		//Julian col & Bomb
		else return true;
	}
	
	bool isFire(){
		if(id == 210 || id == 211 || id == 221) return true;
		else return false;
	}
	
	bool isIce(){
		if(id == 209 || id == 212 || id == 213 || id == 222) return true;
		else return false;
	}
};

//使用梯形
class fuzzyValue{
	string name;
	float height;	//最大值
	float begin;	//起點，此點以下皆為0
	float lb;		//值 = hight 的下限
	float hb;		//值 = hight 的上限
	float end;		//終點，此點以上皆為0
	
	//輸入值必須滿足： rb < rlb < rhb < re
	fuzzyValue(string rn, float rb, float rlb, float rhb, float re, float rh=1.0f){
		name = rn;
		height = rh;
		begin = rb;
		lb = rlb;
		hb = rhb;
		end = re;
	}
	
	/* Implement the copy constructor
    fuzzyValue@ opAssign(const fuzzyValue &in other)
    {
      // Copy the value of the other instance
	  height = other.height;
	  begin = other.begin;
	  end = other.end;
	  lb = other.lb;
	  hb = other.hb;
	  return this;
    }//*/
	
	float fuzzify(float r){
		if(r < begin) return 0;
		else if(r < lb){
			return (r-begin) * height / (lb-begin);
		}
		else if(r < hb) return height;
		else if(r < end){
			return (end-r) * height / (end-hb);
		}
		else return 0;
	}
	
	float deFuzzify(){
		return (begin + lb + hb + end) / 4;
	}
	
	void toScreen(){
		print("fuzzyValue " + name + ": h=" + height + ",\t" + begin + ", " + lb + ", " + hb + ", " + end + "\n");
	}
};

class fuzzySet{
	string name;
	fuzzyValue@[] value;
	//uint top;
	
	fuzzySet(string rn){
		name = rn;
		//value.resize(5);
		//top = 0;
	}
	
	fuzzySet(string rn, fuzzyValue@ r){
		name = rn;
		//value.resize(5);
		//value[0] = r;
		//top = 1;
		value.insertLast(r);
	}
	
	void push(fuzzyValue@ r){
		value.insertLast(r);
	}
	
	float[] fuzzify(float r){
		float[] d;
		for(uint i=0;i<value.length(); i++){
			d.insertLast(value[i].fuzzify(r));
		}
		return d;
	}
	
	float deFuzzify(){
		float d=0;
		float hh=0;
		for(uint i=0;i<value.length(); i++){
			d += value[i].deFuzzify() * value[i].height;
			hh += value[i].height;
		}
		return d/hh;
	}
	
	void toScreen(){
		print("fuzzySet " + name + "{\n");
		for(uint i=0;i<value.length(); i++){
			value[i].toScreen();
		}
		print("}\n");
	}
	
	fuzzyValue@ opIndex(uint r){
		return value[r];
	}
	
	fuzzyValue@ opIndex(string r){
		for(uint i=0; i<value.length(); i++){
			if(value[i].name == r){
				return value[i];
			}
		}
		return null;
	}
	
	uint indexOf(string r){
		for(uint i=0; i<value.length(); i++){
			if(value[i].name == r){
				return i;
			}
		}
		return 2147483647;
	}
};