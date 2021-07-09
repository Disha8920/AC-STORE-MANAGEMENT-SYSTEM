#include<conio.h>
#include<stdio.h>
#include<process.h>
#include<fstream.h>
#include<string.h>
class product
{
	int pno;
	char type[20];
	char brand[50];
	float price,qty,dis;
	public:
	void create_product()
	{
		cout<<"\n Please enter the product no. of the air conditioner:";
		cin>>pno;
		cout<<"\n Please enter the type of the air conditioner:";
		gets(type);
		cout<<"\n Please enter the brand of the air conditioner:";
		gets(brand);
		cout<<"\n Please enter the price of the air conditioner:";
		cin>>price;
		cout<<"\n Please enter the discount (%):";
		cin>>dis;
	}
	void show_product()
	{
		cout<<"\n Product Number: "<<pno;
		cout<<"\n Type: "<<type;
		cout<<"\n Brand: "<<brand;
		cout<<"\n Price: "<<price;
		cout<<"\n Discount(%): "<<dis;
	}
	int retpno()
	{return pno;}
	float retprice()
	{return price;}
	char* rettype()
	{return type;}
	char* retbrand()
	{return brand;}
	float retdis()
	{return dis;}
};


fstream fp; product pr,prod;
void write_product()
{
	fp.open("shop.dat",ios::app|ios::binary);
	pr.create_product();
	fp.write((char*)&pr,sizeof(product));
	fp.close();
	cout<<endl<<"The product has been created.";
	getch();
}
void display_all()
{
	clrscr();
	cout<<"\n\n\t\DISPLAY ALL RECORDS!!!\n\n";
	fp.open("shop.dat",ios::in|ios::binary);
	while(fp.read((char *)&pr,sizeof(product)))
		{
			pr.show_product();
			cout<<"\n\n================================\n";
			getch();
		}

	fp.close();
	getch();
}
void display_sp(int n)
{
	int flag=0;
	fp.open("shop.dat",ios::in|ios::binary);
	while(fp.read((char *)&pr,sizeof(product)))
	{
		if(pr.retpno()==n)
		{
			clrscr();
			pr.show_product();
			flag=1;
		}
	}
	fp.close();
	if(flag==0)
	cout<<endl<<"RECORD NOT FOUND!!!!!";
	getch();
}
void modify_product()
{
	int no,found=0;
	clrscr();
	cout<<endl<<"TO MODIFY :";
	cout<<endl<<"please enter the product no of the product:";
	cin>>no;
	fp.open("shop.dat",ios::in|ios::out|ios::binary);
	while(fp.read((char *)&pr,sizeof(product)) && found==0)
	{
		if(pr.retpno()==no)
		{
			pr.show_product();
			cout<<endl<<"please enter the new details of product:";
			pr.create_product();
			int pos=-1*sizeof(pr);
			fp.seekp(pos,ios::cur);
			fp.write((char *)&pr,sizeof(product));
			cout<<"\n\t record updated";
			found=1;
			}
		}
	fp.close();
	if(found==0)
	cout<<endl<<"record not found";
	getch();
}
void delete_product()
{
	int no;
	char found='f',confirm='n';
	clrscr();
	cout<<"\n\n\t TO DELETE:";
	cout<<"\nPlease enter the product no. of the product you want to delete:";
	cin>>no;
	fp.open("shop.dat",ios::in|ios::out|ios::binary);
	fstream fp2;
	fp2.open("temp.dat",ios::out|ios::binary);
	while(!fp.eof())
	{
		fp.read((char*)&pr, sizeof(pr));

		if(pr.retpno()==no)
		{
			pr.show_product();
			found='t';
			cout<<"\n Are you sure you want to delete this record?(y/n)";
			cin>>confirm;
			if(confirm=='n')
			fp2.write((char*)&pr, sizeof(pr));

		}
		else
		{
		fp2.write((char*)&pr, sizeof(pr));
		}
	}


	if(found=='f')
		cout<<"\n RECORD NOT FOUND!!!";
	fp2.close();
	fp.close();
	remove("shop.dat");
	rename("temp.dat","shop.dat");
	fp.open("shop.dat",ios::in);
	cout<<"\n Now the menu contains:\n";
	while(!fp.eof())
	{
		fp.read((char*)&prod, sizeof(product));
		if(fp.eof())
		break;
		prod.show_product();
		cout<<"\n============================================";
	}
	fp.close();

	getch();
}
void menu()
{
	clrscr();
	fp.open("shop.dat",ios::in|ios::binary);
	if(!fp)
	{
		cout<<"ERROR!!! FILE COULD NOT BE OPENED\n\n\n GO TO ADMIN MENU TO CREATE FILE";
		cout<<"\n\n\n program is closing....";
		getch();
		exit(0);
	}
	cout<<"\n\n\t PRODUCT MENU\n\n";
	cout<<"==================================================================\n";
	cout<<"P.NO \t\tTYPE \t\tBRAND \t\tPRICE\n";
	cout<<"==================================================================\n";
	while(fp.read((char *)&pr,sizeof(product)))
	{
		cout<<pr.retpno()<<"\t\t"<<pr.rettype()<<"\t\t"<<pr.retbrand()<<"\t"<<pr.retprice()<<endl;
	}
	fp.close();
}
void place_order()
{
	int order_arr[50],quan[50],c=0;
	float amt,damt,total=0;
	char ch='y';
	menu();
	cout<<"\n======================================";
	cout<<endl<<"         PLACE YOUR ORDER";
	cout<<"\n=======================================";
	do{
		cout<<endl<<"Enter the product no of the air conditioner:";
		cin>>order_arr[c];
		cout<<endl<<"Quantity in number you want:";
		cin>>quan[c];
		c++;
		cout<<endl<<"Do you want to order another product?(y/n):";
		cin>>ch;
	}
	while(ch=='y'||ch=='Y');
	cout<<endl<<"Thank you for placing your order.";getch();clrscr();
	cout<<"\n\n********************INVOICE******************\n";
	cout<<"\nPr no.\tPr type \tQuantity \tPrice \ttotal \tAmount after discount\n";
	for(int x=0;x<=c;x++)
	{
		fp.open("shop.dat",ios::in|ios::binary);
		fp.read((char *)&pr,sizeof(product));
		while(!fp.eof())
		{
			if(pr.retpno()==order_arr[x])
			{
				amt=pr.retprice()*quan[x];
				damt=amt-(amt*pr.retdis()/100);
				cout<<endl<<order_arr[x]<<"\t"<<pr.rettype()<<"\t\t   "<<quan[x]<<"\t\t"<<pr.retprice()<<"\t"<<amt<<"\t\t"<<damt;
				total+=damt;
			}
			fp.read((char *)&pr,sizeof(product));
			}
		fp.close();
		}
		cout<<"\n\n\n------------------ YOUR BILL IS READY.------------------";
		cout<<"\n\n\tTOTAL="<<total;
		getch();
}
void admin_menu()
{
	clrscr();
	char ch2;
	cout<<"\n\n \t ADMIN MENU";
	cout<<"\n\n\t 1.CREATE PRODUCT";
	cout<<"\n\n\t 2. DISPLAY ALL PRODUCTS";
	cout<<"\n\n\t 3. QUERY";
	cout<<"\n\n\t 4. MODIFY PRODUCT";
	cout<<"\n\n\t 5. DELETE PRODUCT";
	cout<<"\n\n\t 6. VIEW PRODUCT MENU";
	cout<<"\n\n\t 7. BACK TO MAIN MENU";
	cout<<"\n\n\t Please enter your choice(1-7):";
	ch2=getche();
	switch(ch2)
	{
		case '1':	clrscr;
				write_product();
				break;
		case'2':	display_all();
				break;
		case '3':	int num;
				clrscr();
				cout<<"\n\n\t Please enter the product no.:";
				cin>>num;
				display_sp(num);
				break;
		case '4':	modify_product();
				break;
		case '5':	delete_product();
				break;
		case '6':	menu();
				getch();
		case '7':	break;
		default:	cout<<"\a";
				admin_menu();
		}
	}
void main()
{
	char ch,pass[30];
	do
	{
		clrscr();
		cout<<"\n\n\n***************MAIN MENU***************";
		cout<<"\n\n\t 01.CUSTOMER";
		cout<<"\n\n\t 02. ADMINISTRATOR";
		cout<<"\n\n\t 03. EXIT";
		cout<<"\n\n\t Please select your option (1-3):";
		cin>>ch;
		switch(ch)
		{
			case'1': clrscr();
				place_order();
				getch();
				break;
			case'2': cout<<"\n Please enter the password: \t";
				gets(pass);
				if(strcmp("weatherking",pass)==0)
				admin_menu();
				break;
			case'3': exit(0);
			default: cout<<"\a";
		}
	}while(ch!='3');
}












