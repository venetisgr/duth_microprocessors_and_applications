/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
		int *var = (int*)0x1ffff00c  ;      // selection variable 
		int *offset = (int*)0x1ffff010;  // found user offset       
		
	
__asm void index_init(){
  
	LDR R0,=0x1ffff000 ;loads offset index address
	MOVS R1,#0
	STR R1,[R0]  
	
	
	BX LR
}	

__asm void username_create(){
	
	LDR r0 ,=0x1ffff000 ;new user offset index load		
	LDR r1 ,=0x1ffff03f ;base address load
	LDR r2 ,=0x1ffff004 ;HEX keyboard input address
	LDR r3,[r0] ; loads the offset index
	ADDS r1,r3,r1; next available address to store
	LDRB R2,[R2] ;	loads the username value
	STRB r2,[r1];stores the username in new available address strb since username is one byte
	ADDS r3,r3,#1 ; plus one address is now available/empty
	STR r3,[r0] ;stores the new index
	
	BX LR
	
	
	
}

__asm void password_create(){
	
	
	LDR r0 ,=0x1ffff000 ;new user offset index load		
	LDR r1 ,=0x1ffff03f ;base address load
	LDR r2 ,=0x1ffff004 ;HEX keyboard input address
	LDR r3,[r0] ; loads the offset index
	ADDS r1,r3,r1; next available address to store
	LDRB R2,[R2] ;	loads the password value
	STRB r2,[r1];stores the password in new available address strb since password is one byte
	ADDS r3,r3,#1 ; plus one address is now available/empty
	STR r3,[r0] ;stores the new index
	
	BX LR
	
	
}

__asm void permissions_create(){
	
	LDR r0 ,=0x1ffff000 ;new user offset index load		
	LDR r1 ,=0x1ffff03f ;base address load
	LDR r2 ,=0x1ffff004 ;HEX keyboard input address contains permissions
	LDR r3,[r0] ; loads the offset index
	ADDS r1,r3,r1; next available address to store
	LDRB R2,[R2] ;	loads the permissions value
	STRB r2,[r1];stores the permissions in new available address strb since password is one byte
	ADDS r3,r3,#1 ; plus one address is now available/empty
	STR r3,[r0] ;stores the new index
	BX LR
	
	
	
}

__asm void admin_status_enable(){
	LDR r0 ,=0x1ffff000 ;new user offset index load	
	LDR r1 ,=0x1ffff03f ;base address load	
	LDR r2,[r0] ; loads the offset index
	ADDS r1,r2,r1 ;next address available
	MOVS r3,#1 ; 1 corresponds to admin status
	STRB r3,[r1] ; stores the permission which is one byte
	ADDS r2,r2,#1 ; plus one address is now available/empty
	STR r2,[r0] ;stores the new index
	BX LR
	
	
}

__asm void username_check(){
		PUSH{r4-r6,LR}
		MOVS r0,#0 ;local index for loop
		LDR  r1 ,=0x1ffff000 ; New (available) user offset index address
		LDR r1,[r1] ;loads offset index
		LDR r2 ,=0x1ffff03f ;base address
		LDR r4,=0x1ffff004 ; username input from HEX keyboard
		LDRB r4,[r4]
		LDR r5,=0x1ffff00c ;selection variable address
back
		ADDS r6,r2,r0 ;username address 
		LDRB r3,[r6];loads username from RAM
		CMP r3,r4
		BEQ fnd1 ; user was found 
		ADDS r0,r0,#1 ; still searching
		CMP r0,r1
		BLO back
		MOVS r1,#1 ;not found
		STR r1,[r5]
		POP{r4-r6,PC}	
fnd1
		MOVS r1,#0
		STR r1,[r5]
		LDR r5,=0x1ffff010;  ; found user offset
		STR r0,[r5];stores the offset of the current user based on base address
		POP{r4-r6,PC}	
		
	
}

__asm void default_user_perm(){
	LDR r0 ,=0x1ffff000 ;new user offset index load		
	LDR r1 ,=0x1ffff03f ;base address load
	LDR r2,[r0] ; loads the offset index
	ADDS r1,r2,r1; next available address to store
	MOVS r2,#0 ;sets user permissions 
	STRB r2,[r1];stores the permissions in new available address strb since password is one byte
	ADDS r1,r1,#1 ; plus one address is now available/empty
	STR r1,[r0] ;stores the new index
	BX LR
	
	
}

__asm void password_check(){
	LDR r0,=0x1ffff010;  ; found user offset
	LDR r0,[r0]
	ADDS r0,r0,#1 ; plus in order to go to password address
	LDR r1,=0x1ffff03f ;base address
	LDRB r1,[r1,r0] ; password	stored in RAM
	LDR r2,=0x1ffff004  ; password input addr from HEX keyboard
	LDRB r2,[r2] ;hex password
	LDR r3 ,=0x1ffff00c; selection variable 
	CMP r1,r2
	BEQ skp	
	MOVS r0,#1 ; 1 if not found
	STR r0,[r3]
	BX LR
skp
	MOVS r0,#0 ; 0 if  found
	STR r0,[r3]
	BX LR
}

__asm void permission_check(){
		LDR r0,=0x1ffff010;  found user offset
		LDR r0,[r0] ; loads offset 
		ADDS r0,r0,#2 ; plus 2 in order to check permission status
		LDR r1,=0x1ffff03f ;base address
		ADDS r1,r0,r1
		LDRB r2,[r1] ;loads permission
		LDR r3 ,=0x1ffff00c ;selection variable
		;var=1 admin var=0 user
		CMP r2,#1
		BEQ jmp
		MOVS r0,#0
		STR r0,[r3]
		BX LR

jmp	;admin
		MOVS r0,#1
		STR r0,[r3]
		BX LR		
}

__asm void case_select(){
		LDR r0 ,=0x1ffff00c ; selection variable
		LDR r1 ,=0x1ffff008 ; Input address from the second keyboard
		LDRB r1,[r1] ;second keyboard input
		STRB r1,[r0] ; stores option selected from keyboard to  selection variable
		BX LR
}
	
__asm void idle_state_exit(){
		PUSH{LR}
		LDR r0 ,=0x1ffff008  ; Input address from the second keyboard
		LDRB r1,[r0] ; loads selected option 
		LDR r2 ,=0x1ffff00c ; selection variable address  
		CMP r1,#0 ;checks if selected option was 00
		BEQ skip
		MOVS r3,#0 ;0 if its not found 
		STR  r3,[r2]
		
		BX LR	
		//POP{PC}	
skip
		MOVS r3,#1 ;1 if its  found 
		STR  r3,[r2]
		POP{PC}
		//BX LR
}
__asm void password_update(){
			LDR r0, =0x1ffff010 ; Current User offset
			LDR r1, =0x1ffff03f ; Base address
			LDR r2 ,=0x1ffff004 ; Input address hex keyboard
			LDR r0,[r0] ; User offset
			LDRB r2,[r2] ; User input, new password
			ADDS r0,r0,#1 ; plus one to access password address
			STRB r2,[r1,r0]
			BX LR
}

__asm void find_user_position(){
			
			
	PUSH{r4-r5,lr}
		MOVS r0,#0 ;local index for loop
		LDR  r1 ,=0x1ffff000 ; New (available) user offset index address
		LDR r1,[r1] ;loads offset index
		LDR r2 ,=0x1ffff03f ;base address
		LDR r4,=0x1ffff004 ; username input from HEX keyboard
		LDRB r4,[r4]
		LDR r5,=0x1ffff00c ;selection variable address
bck
		ADDS R3,R2,R0
		LDRB r3,[r3];loads username from RAM
		CMP r3,r4
		BEQ fnd2 ;found
		ADDS r0,r0,#3 ; still searching
		CMP r0,r1
		BLO bck
		MOVS r1,#0 ;not found
		STR r1,[r5]
		POP{r4-r5,PC}	
fnd2
		MOVS r1,#1
		STR r1,[r5]
		LDR r5,=0x1ffff010;  ; found user offset
		STR r0,[r5];stores the offset of the current user based on base address
		POP{r4-r5,PC}	
	
		
		
}

__asm void change_permissions(){
			LDR r0 ,=0x1ffff010 ; contains the  offset 
			LDR r0 , [r0] ;
			ADDS r0,r0,#2 ;plus two in order to go to the password position 
			LDR r1, =0x1ffff03f ; Base address
			ADDS r1,r0,r1 ;address of the permissions 
			LDR  r2 ,=0x1ffff004 ;permissions chosen by the user
			LDRB r2,[r2]
			STRB  r2,[r1];stores the new permissions
		
	BX LR
}
__asm void delete_user(){
			PUSH{r4-r5,lr}
			LDR r0 ,=0x1ffff010 ; contains the  offset of the user to be deleted 
			LDRB r0 , [r0] 
			;i need to move all users down 3 slots in order to cover the gap
			LDR r1, =0x1ffff000 ; New User (to be added) offset
			LDRB r1,[r1]
			LDR r2, =0x1ffff03f ; Base address
			ADDS r3,r0,#3 ; next user to be shifted down
loop	LDRB  r4,[r2,r3]
			STRB  r4,[r2,r0]
			ADDS r0,r0,#1
			ADDS r3,r3,#1
			CMP  r3,r1
			BLO loop
			LDR r3, =0x1ffff000 ; New User (to be added) offset
			SUBS r1,r1,#3 ; r1 contains the value while r3 the address of the offset 
			STR r1 ,[r3] ; Since a user is deleted the index moves down
			POP{r4-r5,PC}
}
//
__asm void testvalue1(){   //used in order to check idle status 
	LDR r0 ,=0x1ffff008
	MOVS r1,#0
	STRB r1,[r0]
	
	bx lr
}
__asm void testvalue2(){   //used in order to check username and password
	LDR r0 ,=0x1ffff004
	MOVS r1,#7
	STRB r1,[r0]
	
	bx lr
}

__asm void testvalue3(){   //used in order to check the available options
	LDR r0 ,=0x1ffff008
	MOVS r1,#2
	STRB r1,[r0]
	
	bx lr
}

__asm void testvalue4(){   //used in order to input new password
	LDR r0 ,=0x1ffff004
	MOVS r1,#6
	STRB r1,[r0]
	
	bx lr
}

__asm void testvalue5(){   //used in order to create the new user
	LDR r0 ,=0x1ffff004
	MOVS r1,#9
	STRB r1,[r0]
	
	bx lr
}
__asm void testvalue6(){   //used in order to create the permissions of the new user (admin case)
	LDR r0 ,=0x1ffff004
	MOVS r1,#0
	STRB r1,[r0]
	
	bx lr
}



//
void initialize(){
	//index initialization
	index_init();
	
//	printf("Welcome to admin initialization \n");
//	printf("Please type your username \n");
	
	testvalue2();																					
	username_create();
	
	//printf("Please type your passworld \n");
	
	testvalue2();																					
	password_create();
																								 
	//admin status enable
	admin_status_enable();
	
	//printf("Exiting admin initialization \n");
	
	
	return;
}


int main(void)
{	 
	int *var2=var;
	int control=1;
   initialize();
	 
	while(1){
		
					//printf("IDLE STATE \n");
					//printf("Press 00 to leave idle state\n");
	
					testvalue1();	 //second keyboard input																				
					idle_state_exit();

						
			if((*var2)==0){ // if its 0 it returns to idle state due to wrong(not valid) input
			//printf("Wrong input....returns to idle \n");
			continue;}
			//printf("Please type your username \n");
	
			

			testvalue2();	//username input																		
			username_check();
		if(*var2){ // if its 1 it returns to idle state due to wrong(not valid) input
			//printf("Wrong username....returns to idle \n");		
			continue;
		}
		//printf("Please type your password \n");
			testvalue2();																						
			password_check();
		if(*var2){ // if its 1 it returns to idle state due to wrong(not valid) input
//			printf("Wrong password....returns to idle \n");
			continue;
	}
	  //*********** admin and user options************//
	   permission_check(); //decides if its user or admin 
		//var=1 admin var=0 user
		if(*var2){ //admin
			 while(control){ 
//		printf("Available Options:\n 01 User Exit \n 02 Password Change \n 03 New User Creation \n 04 Delete User \n 05 Admin mode Enable/Disable \n");
//		printf("Type your input \n");
					testvalue3();
				 case_select();  //input selection
				 switch(*var2){
					 case 1:
						 control=0; // 01 exits while and then returns to idle since the loop is over 
						 break;
					 case 2:
						// printf("Type your new password \n");
						testvalue4();
						 password_update();
						 break;
					 case 3:
//						 printf("Type the username of the new user \n");
					 testvalue5();
						username_create();
//						 printf("Type the password of the new user \n");
					 testvalue5();
						password_create();
					//	 printf("Type the permissions of the new user 1 for admin 0 for user \n");
					testvalue6();
					   permissions_create();
						 break;
				   case 4:
				//		 printf("Please type the user to be deleted \n"); //his name
						testvalue5();
						 find_user_position();
						if(*var2==0){
				//			printf("User not found returning to options \n");
							break;}
						 delete_user();
						 break;
					 case 5:
			//			 printf("Please type user to have his permissions changed \n"); //his name
							testvalue5();
						 find_user_position();
						if(*var2==0){
			//				printf("User not found returning to options \n");
							break;}
		//				printf("Please type the permissions you want to change 1 for admin 0 for user \n");
							testvalue6();
						 change_permissions();
						 break;
					default :
  //       printf("Invalid input \n" );
					break;
				 }
		}
	}
		else{ //user
			while(control){ 
	//	printf("Available Options:\n 01 User Exit \n 02 Password Change \n 03 New User Creation \n ");
	//	printf("Type your input \n");
				testvalue3();
				 case_select();//input selection
				 switch(*var2){
					 case 1:
						 control=0; // 01 exits while and then returns to idle since the loop is over 
						 break;
					 case 2:
						 //printf("Type your new password \n");
						testvalue4();
						 password_update();
						 break;
					 case 3:
					//	 printf("Type the username of the new user \n");
						testvalue5();
						username_create();
				//		 printf("Type the password of the new user \n");
					 testvalue5();
						password_create();
						 default_user_perm(); // a user can only create a user
						 break;
					 // normal user doesnt have the option to change permissions or delete users
					default :
        // printf("Invalid input \n" );
					break;
				 }
				}
			}
		control=1; // resets control status in order to be used for the next loop
				
}
		while (1) ;
									}
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
