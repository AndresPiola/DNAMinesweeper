// Copyright 2021 DNABLOCK. All rights reserved.


#include "DNAField.h"

#include "DNABoard.h"
 

void UDNAField::InitializeField(FIntPoint NewBoardPosition, UDNABoard* Board)
{
	BoardPosition=NewBoardPosition;
	BoardReference=Board;
	FieldState=GT_FIELD_HIDDEN;
	BoardReference->OnMouseClickBoard.AddDynamic(this,&ThisClass::MouseClickBoardCallback);
	BoardReference->OnRevealField.AddDynamic(this,&ThisClass::UDNAField::RevealFieldCallback);
	BoardReference->OnRevealAll.AddDynamic(this,&ThisClass::UDNAField::RevealAllCallback);
	NumberMinesAround=0;
}

void UDNAField::SetIndexAndMine(const int32 FieldIndex, const bool bSetAsMined)
{
	Index=FieldIndex;
	bHasMine=bSetAsMined;
}

void UDNAField::MouseClickBoardCallback(bool bWasLeftMouseClicked)
{
	
	//mouse is not over the button so skip
	if(!IsHovered())return;
	//this field is ready
	if(FieldState.MatchesTag(GT_FIELD_REVEALED))return;

	//thios means right click at least
	if(!bWasLeftMouseClicked)
	{
		SwitchFlagState();
		return;
	}

 
	// if(!ensureMsgf(BoardReference!=nullptr,TEXT("BoardReference is null for field %s"),*GetName()))
	// return;

	BoardReference->HandleClickOnField(BoardPosition.X,BoardPosition.Y,Index);
	
	
}

void UDNAField::RevealFieldCallback(int32 IndexFromBoard, int32 Result)
{

	if(bHasMine)return  ;
	if(!RevealField(IndexFromBoard,Result))return;
	
	BoardReference->UpdateFieldCount();
	 
	
}

bool UDNAField::RevealField(int32 IndexFromBoard, int32 Result)
{
	if(IndexFromBoard!=Index)return false;
	OnRevealContent(Result);
	
	
	FieldState=GT_FIELD_REVEALED;
	return true;
}

void UDNAField::RevealAllCallback()
{
	if(FieldState.MatchesTag(GT_FIELD_REVEALED))return;
	
	OnRevealContent(bHasMine?-1:0);
	
}

void UDNAField::SwitchFlagState()
{
	FieldState= FieldState.MatchesTag(GT_FIELD_HIDDEN)?GT_FIELD_FLAGGED:GT_FIELD_HIDDEN;

	FlagIcon->SetVisibility(FieldState.MatchesTag(GT_FIELD_HIDDEN)?ESlateVisibility::Hidden:ESlateVisibility::HitTestInvisible);

	 BoardReference->UpdateFieldCount();
}
