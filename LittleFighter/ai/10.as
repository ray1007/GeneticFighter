//woody
int pre_hp=500;
int elosshp=0;
int recover = 0;
int victory=0;
int start =0;

int input_x = 0;
int input_z = 0;

int prepos = 0;
int stay =1;
array<int> AttSeq = {80 ,8 ,7 ,5 ,2 ,6 ,3 ,7 ,7 ,1 ,1 ,1 ,6 ,1 ,5 ,2 ,2 ,3};
//48,2,7,7,6,1,8,5,5,8,7,4
int finish = 0;
int recordHP;
int run =0;

//int enid = -1;

              
int SeqLen = AttSeq.length();
int countAtt = 1;
int clrVal = 0;
int lasthurt =0;

int obsx = 0;
int obsz = 0;

int eneID = 0;
int countloop = 0;

int pre_mp = 350;
int lossmp = 0;

int doozitone = 0;
int enflying = 0;
int sucatt = 0;
int behurt = 0;

int test_doo = 0;
int test_fly= 0;
int test_cat = 0;
int fly_attack = 0;

void cleanButton(){
	right(0,0);
	left(0,0);
	up(0,0);
	down(0,0);
	J(0,0);
	A(0,0);
	D(0,0);
}

int getfitness(int enhurt,int smp,int sp,int tp,int count){
	int fit = 0;
	int puni = 0;
	if(enhurt > 480)
		puni = abs(input_x);
	fit = (500 - enhurt) -puni + (sucatt*150/SeqLen) + test_doo*3 + fly_attack*25 + test_cat;
	/*print("hp = "+enhurt+"\n");
	print("doo = "+test_doo+"\n");
	print("puni = "+puni+"\n");
	print("success att = "+sucatt+"\n");
	print("SeqLen = "+SeqLen+"\n");
	//print("fly = "+test_fly+"\n");
	print("cat = "+test_cat+"\n");
	print("fly_attack = "+fly_attack+"\n");*/
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
	//clr();
	//print(fly_attack+"\n");
	//print(start);
	//print("frame:"+self.frame+", state:"+self.state+", idx:"+countAtt+", alph:"+AttSeq[countAtt]+"\n");
	if(finish == 21) {
		elosshp = 0;
		pre_hp = 500;
		return;
	}
	countloop++;
	if(SeqLen <=4) {
		for(int i =0;AttSeq.length()<5;i++)
			AttSeq.insertLast(1);
		SeqLen = AttSeq.length();
	}
	input_x = AttSeq[0];
	//if(input_x < 0) input_x = 10;
	
	cleanButton();
	float seed = rand(100);
	seed /= 100;
	person me = person(true);
	person@ competitor = null;

	array<person> enemies;
	array<int> obsc;
	for(int i=0; i<400; i++){
		//剔除自己
		if(loadTarget(i) != -1 ){
			if(target.type == 2 && target.y == 0){
				obsc.insertLast(i);
			}
			if(target.num != self.num ){
				if(target.type == 0){
					float dx = (self.facing ? self.x - target.x : target.x - self.x);
					//敵方
					eneID = i;
					enemies.insertLast( person(false));
					if( @competitor == null || (competitor.state == 14 && target.state != 14) || (competitor.absDx() > abs(dx) && target.state != 14)){
						@competitor = enemies[enemies.length()-1];
						//break;
					}
				}
			}	
		}
	}


	if(competitor.state == 16)
		test_doo++;
	if(competitor.y < 0)
		test_fly++;
	if(competitor.state == 10)
		test_cat++;
	loadTarget(eneID);
	if(competitor.blink > 5 && finish == 0){
		clr();
		return;
	}
	//結束load敵人資料
	if(target.hp >= pre_hp+30){
		recover = 1;
	}
	//print("target x = "+target.x+"\n");
	//print("self x = "+self.x+"\n");
	//print("pre hp = "+pre_hp+"\n");
	//print("eloss hp = "+elosshp+"\n");
	if (target.hp<=pre_hp){
		elosshp += pre_hp-target.hp;
	}
	//pre_hp = target.hp;
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
	if(@competitor == null){
		return;
	}
	if(countloop > 800){
		finish = 21;
		print("0");
		return;
	}
	if( (competitor.state == 14 ||(countAtt >4 && (recover == 1 ||competitor.state == 0))) && start == 1 && competitor.y == 0){

		cleanButton();
		if (target.hp<=pre_hp){
			elosshp += pre_hp-target.hp;
		}
		if(finish == 20){
			elosshp = getfitness(target.hp,self.mp,self.x,target.x,countAtt);
			print(elosshp);
			elosshp = 0;
			pre_hp = 500;
			countAtt = 1;
			finish++;
			start = 0;
			return;
		}
		recordHP = elosshp;
		finish ++;
		return ;
	}
	//clr();
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
	if(  self.x >= (target.x+input_x-2) && self.x <= (target.x+input_x+2) && abs(target.z-self.z)<3 && start ==0){
		start = 1;
		if( target.x-self.x>0 ) {
			right();
			return;
		}
		else if(target.x-self.x<0) {
			left();
			return;
		}
	}
	
	//開始按方向鍵
	/*if(start == 0){
		cleanButton();
		if( (target.x - (self.x + input_x) > 5 || ( self.x<(target.x+input_x-5) && self.x>(target.x+5) )) && 1 > seed){
			right(1,1);
			return;
		}
		else if( (self.x -(target.x+input_x) > 5|| ( target.x<(self.x+ input_x-5)&&target.x>(self.x+5))) && 1 > seed){
			left(1,1);
			return ;
		}
		if( target.z - self.z > 2 && 1 > seed){
			down(1,1);
			return;
		}
		else if(self.z - target.z> 2 && 1 > seed){
			up(1,1);
			return;
		}
	}*/
	if(start == 0){
		cleanButton();
		if( self.x < (target.x+input_x-2) && 1 > seed){
			right(1,1);
			return;
		}
		else if( self.x > (target.x+input_x+2) && 1 > seed){
			left(1,1);
			return ;
		}
		if( target.z - self.z > 2 && 1 > seed){
			down(1,1);
			return;
		}
		else if(self.z - target.z> 2 && 1 > seed){
			up(1,1);
			return;
		}
	}
	//開始按招式
	cleanButton();
	if(start == 0) return;
	if(countAtt >= AttSeq.length()) {
		//print("Attack over\n");
		//clr();
		cleanButton();
		if (target.hp<=pre_hp){
			elosshp += pre_hp-target.hp;
		}
		if(finish == 20){
			elosshp = getfitness(target.hp,self.mp,self.x,target.x,countAtt);
			print(elosshp);
			elosshp = 0;
			pre_hp = 500;
			countAtt = 1;
			start = 0;
			finish++;
			return;
		}
		recordHP = elosshp;
		finish++;
		return ;
	}
	int _d = 0;
	int _a = 0;
	int _j = 0;
	int _up = 0;
	int _down = 0;
	int _goj = 0;
	int _dua = 0;
	int _dda = 0;
	int _dgoj = 0;
	int _dgoa = 0;
	int _go = 0;
	//type assign value
	if(self.y == 0){
		if(me.isRunning()){
			_j = 1;
			_a = 1;
			_d = 1;
			_up = 1;
			_go = 1;
			_goj = 1;
			_down = 1;//可以接招嗎?
			_dda = 1;
			_dua = 1;
			_dda = 1;
			_dgoj = 1;
			_dgoa = 1;
		}
		else if(me.isRolling()){
			_dda = 1;
			_dgoa = 1;
			_a = 1;
			_go = 1;
		}
		else if(self.frame == 121) _a = 1;
		else if(self.state == 15){
			//_go = 1;
			//_goj = 1;
			//_a = 1;
		}
		else if(self.frame == 215){
			_dua = 1;
			_d = 1;
			_dgoj = 1;
			_goj = 1;
			_go = 1;
		}
		else if(self.frame == 219){
			_dua = 1;
			_dgoj = 1;
			_go = 1;
		}
		else if(self.frame == 111) _dua = 1;
		else if(self.frame == 245) _dgoj = 1;
		else if((self.frame>=286 && self.frame<=288)||(self.frame>=301 && self.frame<=303)){
			_dgoa = 1;
			_dua = 1;
			_dda = 1;
			_dgoj = 1;
			_go = 1;
		}
		else if( self.frame == 74) {
			_go = 1;
			_dgoj = 1;
			_dua = 1;
			//_goj = 1;
		}
		else if(self.frame == 270){
			_dua = 1;
			//_goj = 1;
			//_go = 1;
		}
		else if(self.state == 0||self.state == 1){
				_go = 1;
				_d = 1;
				_a = 1;
				_j = 1;
				_up = 1;
				_down = 1;
				_goj = 1;
				_dua = 1;
				_dda = 1;
				_dgoj = 1;
				_dgoa = 1;
		}
	}
	else {
		if(me.isDashing()) _a=1;
		if(me.isFalling()) {
			_a = 1;
			_j = 1;
		}
		if(me.isJumping()){
			_a = 1;
			_go = 1;
		}
	}
	int total = _a+_j+_go+_dgoj+_dua+_dda+_goj;
	if(total == 0) return;
	if(target.state == 16) doozitone++;
	if(target.y<0) enflying++;
	//print(target.hp+","+pre_hp+"\n");
	if(behurt == 1 && target.hp < pre_hp-10){
		behurt = 0;
		sucatt++;
		if(competitor.y!=0) fly_attack++;
	}
	behurt = 0;
	pre_hp = target.hp;
	cleanButton();
	if(AttSeq[countAtt] == 1){
		cleanButton();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 9 && _goj == 1){
		if(target.x - self.x >0) {
			right();
			J();
		}
		if(target.x - self.x <=0) {
			left();
			J();
		}
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 2 && _go == 1){//back
		if(target.x - self.x > 0) LEFT();
		else RIGHT();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 3 &&_go == 1){//forward
		if(target.x - self.x >0) RIGHT();
		else LEFT();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 4 && _j == 1){
		JUM();
		countAtt++;
		return;
	}
	if(AttSeq[countAtt] == 5 && _a==1){
		ATT();
		countAtt++;
		behurt = 1;
		return;
	}
	if(AttSeq[countAtt] == 6 && _dgoj == 1){//虎翔霸
		countAtt++;
		if(me.facing == 1) DrJ();
		else DlJ();
		behurt = 1;
		return;
	}
	if(AttSeq[countAtt] == 7 && _dua == 1){
		countAtt++;
		DuA();
		behurt = 1;
		return;
	}
	if(AttSeq[countAtt] == 8 && _dda == 1){
		countAtt++;
		DdA();
		behurt = 1;
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
