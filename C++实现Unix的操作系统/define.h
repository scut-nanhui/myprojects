#pragma once
#define INODENUM		20    
#define BLOCKNUM		20
#define MAXNAME		20        
#define MAXPWD		20
#define DIRECTNAME	14
#define GROUPNAME		15
#define DIRNUM		63
#define BOOTPOS		1024                     //������
#define SUPERSIZE		1024                 //������
#define INODESIZE		1866752				//inode���С
#define INODE			128					//the size of each inode in the disk
#define BLOCKSTART	1825					//the start pos of block
#define BLOCKSNUM		14560               //���ݿ��С
#define BLOCKSIZE		1024                //���С



//the number of inode in the disk is 14560 ,so the inodesize is 14560*128 and the disk is ((14560*128+3*1024)+(2+14560)*1024)/(1024*1024)16MB