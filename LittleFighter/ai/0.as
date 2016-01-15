int flag = 0;

void cleanButton(){
	right(0,0);
	left(0,0);
	up(0,0);
	down(0,0);
	J(0,0);
	A(0,0);
	D(0,0);
}

void id(){
	//cleanButton();
	if(self.hp == 500) flag = 0;
	//print(self.z+"\n");
	if(self.hp <= 490 && flag == 0){
		if(self.z <420) {
			down(1,1);
			flag = 1;
			return;
		}
		else {
			up(1,1);
			flag = 1;
			return;
		}
	}
	return;
}