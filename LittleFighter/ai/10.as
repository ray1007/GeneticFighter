/*nucleargod*/
//#include "windows.h"
//#include "winuser.h"
int pre_hp=500;
int losshp=0;
//bool victory=0;
int KilledEnemies=0;
int ch=0;
//array<int> items;
//int ego(){
void id(){
	//clr();
	if (self.hp<=pre_hp)	losshp+=pre_hp-self.hp;
	pre_hp=self.hp;
	person me = person(true);
	person@ competitor = null;
	//*
	//print("me:" + " hp=" + self.hp + ", mp=" + self.mp + " , loss hp="+ losshp + " , KilledEnemies="+KilledEnemies +"\n");
	//me.toScreen();
	//print("\n");
	//for(int i=0;i<items.length();i++){
	//	print(items[i] + "\n");
	//}
	//*/
	//array<person> teammates;
	if(ch == 0){
		print("start\n");
		ch = 1;
	}
	bool hasTeammate = false;
	int team = 0;
	array<person> enemies;
	array<oobject> things;
	for(int i=0; i<400; i++){
		//剔除自己
		if(loadTarget(i) != -1 && target.num != self.num ){
			//items.insertLast(target.type);
			float dx = (self.facing ? self.x - target.x : target.x - self.x);
			/*if(target.type == 6 && self.hp < 490){
				print("MIIIIIILK\n");
				float dx = target.x - self.x;
				float dz = target.z - self.z;
				if(me.isDrinking()) return;
				if(me.isObjing()) {
					A();
					return;
				}
				if ( (abs(dx) <10 && abs(dz)<10) ){
					A();
					return;
				}
				if(dx > 0) right();
				else left();
				if(dz > 0) up();
				else down();
			}*/
			if(target.type == 0){								//人
				if(target.team == self.team && target.hp > 0){	//友方
					//teammates.insertLast( person(false));
					hasTeammate = true;
					team++;
				}
				else //if(dx < 1000 && dx > -800){//敵方
				{
					enemies.insertLast( person(false));
					//print(i+"\n");
					if(target.hp<0&&target.blink==15) KilledEnemies++;
					if( @competitor == null || (competitor.state == 14 && target.state != 14) || 
						(competitor.absDx() > abs(dx) && target.state != 14)){
						@competitor = enemies[enemies.length()-1];
					}
				}
			}
		}
	}
	//print("teammates : " + team );
	//print ("\nEnemies : "+enemies.length()+"\n");
	//*清除所有已按下的按鍵
	D(0,0);
    A(0,0);
    J(0,0);
    if(!me.isWalking()) right(0,0);
    if(!me.isWalking()) down(0,0);
    if(!me.isWalking()) left(0,0);
    if(!me.isWalking()) up(0,0);
	//*/
	
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
	
	/* 測試輸出
	x_dis.toScreen();
	y_dis.toScreen();
	z_dis.toScreen();
	//*/
	if( @competitor == null){
		//print("can't find competitor\n");
		if(self.mp > rand(200) + 150 && self.hp > rand(200) + 150){
			DuJ();}
		//else 
		//exit(1);
		return ;
	}
	//判斷自己與對方的距離
	float[] dx = x_dis.fuzzify(competitor.absDx());
	float[] dy = y_dis.fuzzify(competitor.absDy());
	float[] dz = z_dis.fuzzify(competitor.absDz());
	//*
	//print("competitor:\n");
	//competitor.toScreen();
	//print("x_dis: ");
	for(uint i=0;i<dx.length(); i++){
		if(dx[i] > 0){
			//print(x_dis[i].name + "*" + dx[i] + " ");
		}
	}
	//print("\ny_dis: ");
	for(uint i=0;i<dy.length(); i++){
		if(dy[i] > 0){
			//print(y_dis[i].name + "*" + dy[i] + " ");
		}
	}
	//print("\nz_dis: ");
	for(uint i=0;i<dz.length(); i++){
		if(dz[i] > 0){
			//print(z_dis[i].name + "*" + dz[i] + " ");
		}
	}
	//print("\n");
	//*/
	float seed = rand(100);
	seed /= 100;
	//print("seed = " + seed + "\n");
	
	//*未判斷風險，不要命打法/
	
	if( competitor.state == 14 || competitor.blink > 5){
		//print("competitor is unhurtable\n");
		//if(competitor.dx > 0) left();
		//else right();
		right();
		if(competitor.dz > 0) down();
		else up();
	}
	else if(self.y == 0){	//地面上
		//print("on groung\n");
		if(me.isRunning()){
			float _up = 0;
			float _down = 0;
			float _d = 0;
			float _j = 0;
			float _a = 0;
			float _stop = 0;
			float[] com_zd = z_dis.fuzzify(abs(competitor.z-bg_zwidth1));
			float[] com_zu = z_dis.fuzzify(abs(competitor.z-bg_zwidth2));
			//print("isRunning, com_zd=" + com_zd[0] + ", com_zu=" + com_zu[0] + "\n");
			//上下移動
			if(dx[3] > seed){		//當作遠，避免同排
				if(competitor.dz > 0){
					_up = com_zu[0];
					_down = _up >= dx[3] ? 0 : dx[3];
				}
				else{
					_down = com_zd[0];
					_up = _down >= dx[3] ? 0 : dx[3];
				}
			}
			else{						//當作不遠，朝對方接近
				if(competitor.dz > 10) _up = 1 - dx[3];
				else if(competitor.dz < -10) _down = 1- dx[3];
			}
			
			//敵人接近，攻擊，取交集
			_a = (dx[0] < dy[0] ? (dx[0] < dz[0] ? dx[0] : dz[0]) : (dy[0] < dz[0] ? dy[0] : dz[0]));
			//敵人x中遠程且z不遠，起跳準備攻擊，取交集
			_j = dx[2] < 1 - dz[2] ? dx[2] : 1 - dz[2];
			//敵人很遠//但Z軸相近
			//if(dx[3] < dz[0]){
				if(dx[3] > _j) _j = dx[3];
			//}
			//else{
			//	if(dz[0] > _j) _j = dz[0];
			//}
			//敵人X接近但Z保持上下不接近，煞車
			_stop = dx[0] < 1 - dz[0] ? dx[0] : 1 - dz[0];
			//敵人在X中近程以及Z不遠且攻擊中，滾地迴避
			if(competitor.isAttacking()) _d = dx[1] < 1 - dz[0] ? dx[1] : 1 - dz[0];
			//目標在後方，滾地
			else if(competitor.dx * me.facing < 0) _d = 1 - dx[0];
			//危險接近，未判定
			
			if(_up > _down && _up > seed) up();		//執行動作
			else if(_down > seed) down();
			
			if(_d > seed) D();
			else if(_j > seed) J();
			else if(_a > seed) A();
			else if(_stop > seed){
				if(self.facing) right();
				else left();
			}
		}
		else if(me.isRolling()){	//滾地，前方有人可以打，自己有氣就使用鬼車腿
			if(self.mp > 50 && !competitor.isAttacking() && dx[1] > seed && competitor.absDz() < 25) DdA();
		}
		else if(me.isCatching()){
			if(self.ctimer < 30 + rand(40)) {
				DuA();
			}
			else A();
		}
		else if(self.frame == 215){	//一般著地，危險時滾，只要敵人不遠，有氣就虎翔霸，否則飛踢
			if(dx[0] > seed && dz[0] > seed && dy[1] > seed) DuA();
			else if(dx[0] > seed && dz[0] > seed && competitor.isAttacking()) D();
			else if((1 - dx[3] > seed) && (1 - dz[2] > seed)){
				if(self.mp > 200 && (dx[0] > seed || competitor.isAttacking() || rand(500) > self.mp)){
					if(competitor.dx > 0) DrJ();
					else DlJ();
				}
				else {
					if(competitor.dz > 25) up(1,1);
					else if(competitor.dz < -25) down(1,1);
					if(competitor.dx > 0){
						right(1,1);
						J();
					}
					else {
						left(1,1);
						J();
					}
				}
			}
		}
		else if(self.frame == 219){	//衝跳著地或者滾地起身，有機會就虎翔霸
			if(dx[0] > seed && dz[0] > seed && dy[1] > seed) DuA();
			else if((1 - dx[3] > seed) && (1 - dz[2] > seed)){
				if(self.mp > 200 && (dx[0] > seed || competitor.isAttacking() || rand(500) > self.mp)){
					if(competitor.dx > 0) DrJ();
					else DlJ();
				}
			}
		}
		else if(me.isObjing()){		//負重，馬上丟
			if(competitor.dx > 0) right(1,1);
			else left(1,1);
			if(competitor.dz > 20) up(1,1);
			else if(competitor.dz < -20) down(1,1);
			A();
		}
		else if(me.isDrinking()){	//喝飲料，有威脅就閃
			if((dx[0] > seed || dx[1] > seed) && dz[0] > seed){
				if(self.mp > 50 && (dx[0] > dx[1] || competitor.dx * me.facing > 0)) DdA();
				else DuA();
			}
			else if(dx[2] > seed && 1 - dz[2] > seed && abs(competitor.vx) > 10 &&
					competitor.dx * competitor.vx < 0 && competitor.vz * competitor.dz <= 0){
				DuA();
			}
		}
		else if(self.frame == 111){	//防住一次攻擊
			DuA();
		}
		else if(self.frame == 245){	//發完波
			if(self.mp > 200 && (dx[0] > seed || dx[1] > seed) && dz[0] > seed){
				if(competitor.dx > 0) DrJ();
				else DlJ();
			}
		}
		else if(self.frame >= 250 && self.frame <= 253){					//虎翔霸調整方向
			if(competitor.dz > 25) up(1,1);
			else if(competitor.dz < -25) down(1,1);
		}
		else if((self.frame >= 286 && self.frame <= 288) || (self.frame >= 301 && self.frame <= 303)){
			//剛傳送到目的地
			if(dx[0] > seed && dz[0] > seed){								//極接近目標
				if(self.mp > 325 && (competitor.isFainting() || competitor.isDrinking())){
					if(competitor.dx > 30) DrA();
					else if(competitor.dx < -30) DlA();
					else DuA();
				}
				else DuA();
			}
			else if(self.mp > 200 && 1 - dx[3] > seed && 1 - dz[2] > seed){	//與目標有段距離但不遠
				if((competitor.isFainting() || competitor.isDrinking()) && dx[1] > seed){	//發現暈眩目標，準備連技
					if( self.mp > 325){				//D>AJ
						if(competitor.dx > 0) DrA();
						else DlA();
					}								//DvA + D^A
					else if(competitor.dx > 0){
						DdA();
					}
				}
				else{
					if(competitor.dx > 0) DrJ();
					else DlJ();
				}
			}
		}
		else{						//其他地面狀態，如站立、行走
			float _up = 0;
			float _down = 0;
			float _left = 0;
			float _right = 0;
			float _run = 0;
			float _d = 0;
			float _j = 0;
			float _a = 0;
			float _dua = 0;
			//float _duj = 0;
			float _dfa = 0;		//波
			float _dfj = 0;		//虎翔霸
			float _dda = 0;
			//float _ddj = 0;
			float _catch = 0;
			
			//移動方向決策
			if( competitor.dx > 0 ){
				_right = 1;
			}
			else{
				_left = 1;
			}
			if(competitor.dz > 0) _up = 1;
			else _down = 1;
			_run = dx[2] > dx[3] ? dx[2] : dx[3];
			//敵人接近，暈眩則抓，否則攻擊，取交集
			if(competitor.isFainting()){
				_catch = (dx[0] < dy[0] ? (dx[0] < dz[0] ? dx[0] : dz[0]) : (dy[0] < dz[0] ? dy[0] : dz[0]));
			}
			else {
				_a = (dx[0] < dy[0] ? (dx[0] < dz[0] ? dx[0] : dz[0]) : (dy[0] < dz[0] ? dy[0] : dz[0]));
			}
			//敵人從空中接近，腳刀，取交集
			_dua = (dx[0] < dy[1] ? (dx[0] < dz[0] ? dx[0] : dz[0]) : (dy[1] < dz[0] ? dy[1] : dz[0]));
			//敵人暈眩且自己氣很多，D>AJ 連技
			if((competitor.isFainting()  || competitor.isDrinking()) && self.mp > 325){
				_dfa = dx[1];
			}
			//敵人在X中近程或近程且在Z中程，跳
			if(dx[0] > dx[1]) _j = dx[0] < dz[1] ? dx[0] : dz[1];
			else _j = dx[1] < dz[1] ? dx[1] : dz[1];
			//敵人在X中近程且暈眩或負重，鬼車腿或者衝過去
			if(competitor.isFainting() || competitor.isObjing() || competitor.isDrinking()){
				if(self.mp > 50 && competitor.absDz() < 25) _dda = dx[1];
				else if(_run < dx[1]) _run = dx[1];
			}
			//敵人在X中遠程且Z不遠，虎翔霸或者跑步準備飛踢
			if(self.mp > 200 && rand(400) > self.hp){
				_dfj = dx[2] < 1 - dz[2] ? dx[2] : 1 - dz[2];
			}
			else{
				float tmp = dx[2] < 1 - dz[2] ? dx[2] : 1 - dz[2];
				if(tmp > _run) _run = tmp;
			}
			
			//敵我距離遙遠，瞬間移動或者發波或者跑步
			if(self.hp > 250 && self.mp > 250 && dx[3] > seed){		//血多氣足，瞬移敵方
				left(0,0);
				right(0,0);
				if(me.facing == competitor.facing) DuJ();
				else if(self.facing) right();
				else left();
				return ;
			}
			else if(hasTeammate && self.hp < 150 && self.mp > 50 &&	//血少，瞬移己方
					rand(500) < self.hp && dx[3] > seed){
				DdJ();
				return ;
			}
			else if(self.mp > 125 && _dfa < dx[3] && rand(500) < self.mp){	//遠程發波
				_dfa = dx[3];
			}
			else if(_run < dx[3]) _run = dx[3];
			
			//開始按方向鍵
			if(_right > _left && _right > seed){
				right();
				_right = 2;
				if( _run > _dfj && _run > seed){
					right();
					return ;
				}
			}
			else if(_left > seed){
				left();
				_left = 2;
				if( _run > _dfj && _run > seed){
					left();
					return ;
				}
			}
			if(_up > _down) up();
			else down();
			//開始按招式
			if(_dfj > seed){
				if(competitor.dx > 0) DrJ();
				else DlJ();
			}
			else if(_dfa > seed){
				if(competitor.dx > 0) DrA();
				else DlA();
			}
			else if(_dda > seed) DdA();
			else if(_dua > seed) DuA();
			else if(_catch > seed){
				if(_left != 2 && _right != 2 ){
					if(competitor.dx > 10) right();
					else if(competitor.dx < 10) left();
				}
			}
			else if(_a > seed) A();
			else if(_d > seed) D();
			else if(_j > seed) J();
		}
	}
	else{				//空中
		//print("in the air\n");
		if(me.isDashing()){
			if(dx[0] > seed) A();
			else if(competitor.dx > 0 && competitor.nextDx() < 200 ) A();
		}
		else if(me.isFalling()){
			if(competitor.state == 301) J();
			else if(seed < 0.2){J();}
		}
		else if(me.isJumping()){
			float _a = 0;
			float _duj = 0;
			float _ddj = 0;
			//敵人接近，攻擊
			if(dx[0] < dz[0]) _a = dx[0] < dy[0] ? dx[0] : dy[0];
			else _a = dz[0] < dy[0] ? dz[0] : dy[0];
			//敵人很遠，傳送
			if(self.mp > 50){
				if(self.hp > rand(300) + 100 && self.mp > rand(125) + 50){
					if(dx[3] > dx[2]) _duj = dx[3] > dz[2] ? dx[3] : dz[2];
					else _duj = dx[2] > dz[2] ? dx[2] : dz[2];
				}
				if(hasTeammate && self.hp < rand(100) + 50){
					if(dx[3] > dx[2]) _ddj = dx[3] > dz[2] ? dx[3] : dz[2];
					else _ddj = dx[2] > dz[2] ? dx[2] : dz[2];
				}
			}
			
			if(_a > seed && (me.vy * competitor.dy > -rand(10))){
				if(competitor.dx > 0) right();
				else left();
				A();
			}
			else if(_duj > seed) DuJ();
			else if(_ddj > seed) DdJ();
			else{
				if(competitor.dx > 0) right();
				else left();
			}
		}
	}

	//*/
	//if (target.hp<10) KilledEnemies++;
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
	float x;		//目標位置
	float y;
	float z;
	float dx;		//以自己為原點的目標位置
	float dy;
	float dz;
	float vx;		//目標移動向量
	float vy;
	float vz;
	
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
}

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
}

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
}