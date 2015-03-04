#pragma once
enum AIGOALSTATUS
{
	INACTIVE, 
	ACTIVE, 
	COMPLETED,
	FAILED
};

/*
 *INACTIVE : The Goal has not been activated yet.
 *ACTIVE : The goal has been activated and will be processed each update.
 *COMPLETED : The goal has been completed and will be removed next update.
 *FAILED : The goal has failed and will be either preplanned on removed on update.
 */