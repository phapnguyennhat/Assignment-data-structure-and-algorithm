#include "main.h"


class imp_res : public Restaurant
{
	private:
		customer* positionX;
		customer* headCustomerInQueue;
		customer* orderInDesk;
		int countInDesk;
		int countInQueue;

	private:
		void remove(customer *& head,customer* cus){
			// xóa phần tử cus trong danh sach lien ket (inDesk,queue,order)
			// if(head->next==head && head==cus){
			// 	head->next=nullptr;
			// 	head->prev=nullptr;
			// 	delete head;
			// 	countInDesk=(head==positionX)? 0:countInDesk;
			// 	countInQueue=(head==headCustomerInQueue)? 0:countInQueue;
			// 	head=NULL;
			// 	return;
			// }
			countInDesk=(head==positionX)? countInDesk-1:countInDesk;
			countInQueue=(head==headCustomerInQueue)? countInQueue-1:countInQueue;
			// positionX=(head==positionX	&&cus->energy>0) ?cus->next:cus->prev; 
			// head=(head==cus)? head->next:head;
			if(head==positionX && cus->energy>0) positionX=cus->next;
			else if(head==positionX && cus->energy<0) positionX=cus->prev;
			else if(head==cus) head=head->next;
			cus->prev->next=cus->next;
			cus->next->prev=cus->prev;
			cus->next=nullptr;
			cus->prev=nullptr;
			delete cus;
			headCustomerInQueue=(countInQueue==0)? NULL:headCustomerInQueue;
			positionX=(countInDesk==0)? NULL:positionX;
			orderInDesk=(countInDesk==0)? NULL: orderInDesk;
		}
		void insert_next(customer*& pos,customer* cus){
			if(pos==NULL){
				pos=cus;
				cus->next=cus;
				cus->prev=cus;
			}
			else{
				cus->prev=pos;
				cus->next=pos->next;
				pos->next->prev=cus;
				pos->next=cus;
			}
			// countInDesk=(pos==positionX)? countInDesk+1:countInDesk;
			// positionX=(pos==positionX)? cus:positionX;
			countInQueue=(pos==headCustomerInQueue)? countInQueue+1: countInQueue;
		}
		void insert_pre (customer*& pos,customer* cus){
			if(pos==NULL){
				pos=cus;
				cus->next=cus;
				cus->prev=cus;
			}
			else{
				cus->next=pos;
				cus->prev=pos->prev;
				pos->prev->next=cus;
				pos->prev=cus;
			}
			// positionX=(pos==positionX)? cus:positionX;
			// countInDesk=(pos==positionX)? countInDesk+1:countInDesk;
			countInQueue=(pos==headCustomerInQueue)? countInQueue+1: countInQueue;
		}
		customer* customerAt(customer* head,int index){

			for(int i=0;i<index;i++){
				head=head->next;
			}
			return head;
		}
		void swap(customer* cus1,customer* cus2){
			string tmp=cus1->name;
			cus1->name=cus2->name;
			cus2->name=tmp;
			int e=cus1->energy;
			cus1->energy=cus2->energy;
			cus2->energy=e;
		}
		int inssort2(customer* head,int size,int incr){
			// sắp xếp theo thứ tự giảm dần của sublist
			// trả về số swap thực hiện
			int res=0;
			for(int i=incr;i<size;i+=incr){
				for(int j=i;j>=incr&& abs(customerAt(head,j)->energy)>abs(customerAt(head,j-incr)->energy);j-=incr){
					// swap
					res++;
					swap(customerAt(head,j),customerAt(head,j-incr));
				}
			}
			return res;
		}
		int shellsort(customer* head,int size){
			int res=0;
			for(int i=size/2;i>2;i/=2){
				for(int j=0;j<i;j++){
					res+=inssort2(customerAt(head,j),size-j,i);
				}
			}
			res+=inssort2(head,size,1);
			return res;
		}
		void kickCustomer(bool isOanLinh)
		{
			// đuổi khách hàng từ vị khách mới đến cho đến kh đến sớm nhất
			if(headCustomerInQueue!=NULL){
				customer*tmp=headCustomerInQueue->prev;
				do{
					bool check=(isOanLinh)? tmp->energy<0:tmp->energy>0;
					if(check){
						cout<<tmp->name<<"-"<<tmp->energy<<"\n";
						tmp=tmp->next;
						remove(headCustomerInQueue,tmp->prev);
					}
					tmp=tmp->prev;
				}
				while(headCustomerInQueue!=NULL&&tmp!=headCustomerInQueue->prev);
			}
			if(orderInDesk!=NULL){
				customer*tmp=orderInDesk->prev;
				do{
					bool check=(isOanLinh)? tmp->energy<0:tmp->energy>0;
					if(check){
						cout<<tmp->name<<"-"<<tmp->energy<<"\n";
						customer* delCus=positionX;
						do{
							if(delCus->name==tmp->name) break;
							delCus=delCus->next;
						}
						while(delCus!=positionX);
						tmp=tmp->next;
						// loi cho nay

						remove(orderInDesk,tmp->prev);
						remove(positionX,delCus);
					}
					tmp=tmp->prev;
				}
				while(orderInDesk!=NULL&&tmp!=orderInDesk->prev);
			}
		}
		void printSubList(int indexBegin,int size){
			// indexbegin la vi tri bat dau tinh tu position x
			// size kich thuoc sublist
			int indexMin=0;
			int minE=customerAt(positionX,indexBegin)->energy;
			for(int i=indexBegin;i<size+indexBegin;i++){
				if(customerAt(positionX,i)->energy<minE){
					minE=customerAt(positionX,i)->energy;
					indexMin=i;
				}
			}
			int index=indexMin;
			do{
				if(index>=indexBegin&&index<indexBegin+size){
					cout<<customerAt(positionX,index)->name<<"-"<<customerAt(positionX,index)->energy<<"\n";
				}
				index=(index+1)%countInDesk;
			}
			while(index!=indexMin);
		
		}
	public:	
		imp_res() {
			positionX=NULL;
			headCustomerInQueue=NULL;
			orderInDesk=NULL;
			countInDesk=0;
			countInQueue=0;
		}
		~imp_res(){
			while(positionX!=NULL){
				remove(positionX,positionX);
			}
			while(orderInDesk!=NULL){
				remove (orderInDesk,orderInDesk);
			}
			while(headCustomerInQueue!=NULL){
				remove(headCustomerInQueue,headCustomerInQueue);
			}
			
		}
		
		void RED(string name, int energy)
		{
			// reject energy equal 0
			if(energy==0) return;
			// thiên thượng điện hạ, duy ngã độc tôn
			// reject customer whose name is on the desk or the queue
			if(positionX!=NULL){
				customer*tmp=positionX;
				do{
					if(tmp->name==name) return;
					tmp=tmp->next;
				}
				while(tmp!=positionX);
			}
			if(headCustomerInQueue!=NULL){
		
				customer*tmp=headCustomerInQueue;
				do{
					if(tmp->name==name) return;
					tmp=tmp->next;
				}
				while(tmp!=headCustomerInQueue);
			}

			cout<<name<<" "<<energy<<endl;
			customer*cus=new customer(name,energy,nullptr,nullptr);
			customer*cusOrder=new customer(name ,energy,nullptr,nullptr);
			// đưa khách vào bàn hoặc vào hàng đợi
			if(countInDesk<MAXSIZE/2){
				// chen khach vao ban
				insert_pre(orderInDesk,cusOrder);		//push back
				if(positionX==NULL||energy>=positionX->energy){
					// chieu dong ho
					insert_next(positionX,cus);
				}
				else if(positionX==NULL||energy<positionX->energy){
					// nguoc chieu dong ho
					insert_pre(positionX,cus);
				}
				
				positionX=cus;
				countInDesk++;
			}
			else{
				if(countInDesk==MAXSIZE && countInQueue<MAXSIZE){
				
					insert_pre(headCustomerInQueue,cus);
				}
				else if(countInDesk<MAXSIZE){
					// dua vao ban theo chien thuat 2
					insert_pre(orderInDesk,cusOrder);
					int sub=0;
					customer* RES=positionX;
					customer* tmp=positionX;
					do
					{
						if (abs(energy - tmp->energy) > abs(sub))
						{
							sub = energy - tmp->energy;
							RES = tmp;
						}
						tmp = tmp->next;
					}
					while(tmp!=positionX);

					if(sub<0){
						insert_pre(RES,cus);
					}
					else{
						insert_next(RES,cus);
					}
					positionX=cus;
					countInDesk++;
				}
			}
		}
		void BLUE(int num)
		{
			cout << "blue "<< num << endl;

			if(num>=countInDesk){
				while(positionX!=NULL){
					remove(positionX,positionX);
					remove(orderInDesk,orderInDesk);
				}
			
			}
			else{
				while(num!=0){
					customer* tmp=positionX;
					do{
						if(tmp->name==orderInDesk->name){
							// popfront orderindex
							remove(orderInDesk,orderInDesk);
							break;

						}
						tmp=tmp->next;
					}
					while(tmp!=positionX);
					
					remove(positionX,tmp);
					num--;
				}
			}

			while(countInDesk<MAXSIZE && headCustomerInQueue!=NULL){
				string name=headCustomerInQueue->name;
				int energy=headCustomerInQueue->energy;
				remove(headCustomerInQueue,headCustomerInQueue);
				RED(name,energy);
			}
			
		}
		void PURPLE()
		{
			cout << "purple"<< endl;
			// hang cho
			// tìm index có energy cao nhất đầu tiên
			// dùng shell sort sắp xếp từ đầu hàng chờ đến index
			// đếm số lần swap (count) sau đó dùng lệnh blue(count%MAXSIZE)
			customer* tmp=headCustomerInQueue;
			int absMaxEnergy=abs(headCustomerInQueue->energy);
			int indexMax=0;
			int index=0;
			do{
				if(abs(tmp->energy)>abs(absMaxEnergy)){
					absMaxEnergy=abs(tmp->energy);
					indexMax=index;
				}
				tmp=tmp->next;
				index++;
			}
			while(tmp!=headCustomerInQueue);
			int N=shellsort(headCustomerInQueue,indexMax+1);
			BLUE(N%MAXSIZE);
		}
		void REVERSAL()
		{
			cout << "reversal" << endl;
			// từ vị trí positionX
			// tạo 2 biến 1 biến cùng dấu position 1 biến khác dấu
			// 
			customer* sameSign=positionX;
			customer* tmp=positionX->next;
			customer* difSign=positionX;
			do{
				difSign=difSign->prev;
				if (sameSign==difSign){
					// tat ca deu duong hoac tat ca deu am
					difSign=NULL;
					break;
				}
			}
			while(sameSign->energy*difSign->energy>0);
			
			for(int i=0;i<countInDesk/2;i++){
				if(sameSign->energy*tmp->energy>0){
					swap(sameSign,tmp);
					// di chuyen den vi tri cung dau position tiep theo
					do{
						sameSign=sameSign->prev;
						// if(sameSign==tmp) break;
					}
					while(sameSign->energy*positionX->energy<0);

				}

				if(difSign!=NULL&&difSign->energy* tmp->energy>0){
					swap(difSign,tmp);
					// di chuyen den vi tri nguoc dau position tiep theo
					do{
						difSign=difSign->prev;
						// if (difSign==tmp) break;
					}
					while(difSign->energy*positionX->energy>0);

				}
				tmp=tmp->next;
			}
			
		}
		void UNLIMITED_VOID()
		{
			cout << "unlimited_void" << endl;
			// tìm dãy con liên tiếp dài nhất (có độ dài tối thiểu là 4)mà có tổng energy bé nhất
			// sau đó in thông tin dãy vừa tìm đc bắt đầu từ phần tử bé nhất "name-energy\n"
			// trường hợp nhiều dãy con thỏa thì lấy dãy cuối cùng tìm đc

			if(countInDesk<4) return;
			int indexBegin=0;
			int size=0;
			int sizeMax=0;
			int sumMin=INT_MAX;
			int sum=0;
			for(int i=0;i<=countInDesk-4;i++){
				customer*tmp=customerAt(positionX,i);
				size=0;
				sum=0;
				
				while(size<4||(tmp->energy<0&&tmp!=positionX)){
					sum+=tmp->energy;
					size++;
					tmp=tmp->next;
				}
				// if(tmp->next==customerAt(positionX,i)&&sum<=sumMin){
				// 	printSubList(indexBegin,size);
				// 	return;
				// }
				// while(tmp->energy<0&&tmp!=positionX){
				// 	sum+=tmp->energy;
				// 	size++;
				// 	tmp=tmp->next;
				// }
				if(sum==sumMin&&size>=sizeMax){
					sizeMax=size;
					indexBegin=i;
				}
				if(sum<sumMin){
					sizeMax=size;
					sumMin=sum;
					indexBegin=i;
				}
			}
			printSubList(indexBegin,sizeMax);
		}


		void DOMAIN_EXPANSION()
		{
			cout << "domain_expansion" << endl;
			// tính tổng energy của chú linh và chú thuật sư tổng trị td ai bé hơn sẽ bị đuỏi về
			// và in ra thông tin "name-energy\n" từ kh đến sau đến kh đến đầu tiên
			// tức là đuổi từ queue đén trong bàn
			// sau đó bố trí khách từ hàng đợi vào chô trống
			customer* desk=positionX;
			customer* queue=headCustomerInQueue;
			int eThuatSu=0;
			int eOanLinh=0;
			if(desk!=NULL){
				do{
					if(desk->energy<0) eOanLinh+=desk->energy;
					else eThuatSu+=desk->energy;
					desk=desk->next;
				}
				while(desk!=positionX);
			}
			if(headCustomerInQueue!=NULL){
				do{
					if(queue->energy<0) eOanLinh+=queue->energy;
					else eThuatSu+=queue->energy;
					queue=queue->next;
				}
				while(queue!=headCustomerInQueue);
			}
			kickCustomer(abs(eThuatSu)>=abs(eOanLinh));
			while(countInDesk<MAXSIZE&&headCustomerInQueue!=NULL){
				string name=headCustomerInQueue->name;
				int energy=headCustomerInQueue->energy;
				remove(headCustomerInQueue,headCustomerInQueue);
				RED(name,energy);
			}
		}
		void LIGHT(int num)
		{
			cout << "light " << num << endl;
			if(num==0&&headCustomerInQueue!=NULL){
				customer* tmp=headCustomerInQueue;
				do{
					cout<<tmp->name<<"-"<<tmp->energy<<"\n";
					tmp=tmp->next;
				}
				while(tmp!=headCustomerInQueue);
			}
			if(num>0&&positionX!=NULL){
				customer*tmp=positionX;
				do{
					cout<<tmp->name<<"-"<<tmp->energy<<"\n";
					tmp=tmp->next;
				}
				while(tmp!=positionX);
			}
			else if(num<0&&positionX!=NULL){
				customer*tmp=positionX;
				do{
					cout<<tmp->name<<"-"<<tmp->energy<<"\n";
					tmp=tmp->prev;
				}
				while(tmp!=positionX);
			}
		}
};