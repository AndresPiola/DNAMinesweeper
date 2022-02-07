// Copyright 2021 DNABLOCK. All rights reserved.


#include "DNABoard.h"

#include "DNAField.h"
#include "Components/UniformGridSlot.h"
#include "Components/WrapBox.h"
#include "Kismet/KismetMathLibrary.h"



void UDNABoard::NativePreConstruct()
{
	Super::NativePreConstruct();
	
 check(GenerateFieldWidgets());
	CurrentGameStatus=GT_GAME_STATUS_WAITING;
 
}
 
FReply UDNABoard::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsLeftMouseClicked= InMouseEvent.GetEffectingButton().GetFName().IsEqual(TEXT("LeftMouseButton"));
	CheckList.Empty();
	
	if(OnMouseClickBoard.IsBound())
	{
		OnMouseClickBoard.Broadcast(bIsLeftMouseClicked);
	 
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}


void UDNABoard::HandleClickOnField(const int32 ColIndex, const int32 RowIndex, const int32 FieldIndex)
{
	if (!bBoardGenerated)
	{
		GenerateBoard(ColIndex, RowIndex);
		ChangeGameStatus(GT_GAME_STATUS_BEGIN);
	}

	
	 ///this is gameover
	if (FieldArray[FieldIndex])
	{
		if(OnRevealAll.IsBound())
			OnRevealAll.Broadcast();
		return;
	}
		
	CheckList.Add(FieldIndex);
	 
	const int32 MinesAround= WidgetsField[FieldIndex]->NumberMinesAround;
	
 
	 	if(OnRevealField.IsBound())
	 		OnRevealField.Broadcast(FieldIndex, MinesAround);
		
	if(MinesAround==0)
	{
		LookAround(ColIndex,RowIndex);
		
	}
	  
	 
}

int32 UDNABoard::CalculateFieldNumber(int32 ColIndex, int32 RowIndex)
{
	const int32 IndexToCheck=CalcIndex(ColIndex, RowIndex);
	if(!CheckIsValidIndex(ColIndex,RowIndex)) return -1;
	if (FieldArray[IndexToCheck])
	{
		return -1;
	}

	return GetValue(ColIndex - 1, RowIndex - 1) + 
		   GetValue(ColIndex - 1, RowIndex) +
		   GetValue(ColIndex - 1, RowIndex + 1) +
		   GetValue(ColIndex, RowIndex - 1) +
		   GetValue(ColIndex, RowIndex + 1) +
		   GetValue(ColIndex + 1, RowIndex - 1) +
		   GetValue(ColIndex + 1, RowIndex) +
		   GetValue(ColIndex + 1, RowIndex +1);
}

void UDNABoard::LookAround(int32 ColIndex, int32 RowIndex)
{
	for (int x = -1; x <= 1; ++x)
		for (int y = -1; y <=1; ++y)
		{
			if(x==0 && y==0)continue;
			const int32 TestX=ColIndex+x;
			const int32 TestY=RowIndex+y;
			const int32 FieldIndex=CalcIndex(TestX,TestY);
			if(!CheckIsValidIndex(TestX,TestY))continue;
			//already Check
			if(CheckList.Contains(FieldIndex))continue;
			CheckList.Add(WidgetsField[FieldIndex]->Index);
			const int32 Result= WidgetsField[FieldIndex]->NumberMinesAround;
			
			 
			if(WidgetsField[FieldIndex]->FieldState.MatchesTag(GT_FIELD_HIDDEN))
			{
				WidgetsField[FieldIndex]->RevealField(FieldIndex,Result);
			}
			if(WidgetsField[FieldIndex]->bHasMine)continue;
			
			if(Result==0)
			LookAround(TestX,TestY);
		}
 
	 
}

bool UDNABoard::GenerateFieldWidgets()
{
	if(FieldClass==nullptr)return  false;
	if (Fields == nullptr)return false;
	 
	if (RowNum < 0 || ColumnNum < 0)
	{
		return false;
	}

	Fields->ClearChildren();
	WidgetsField.Empty();
	WidgetsField.SetNum(ColumnNum*RowNum);
	for (int32 x = 0; x < ColumnNum ; ++x)
	for (int32 y = 0; y < RowNum ; ++y)
		{
	 
			UDNAField* FieldWidget = CreateWidget<UDNAField>(this,FieldClass);
			FieldWidget->InitializeField(FIntPoint(x,y),this);
			 FieldWidget->Index=CalcIndex(x,y); 
			
				Fields->AddChild(FieldWidget);
			WidgetsField[FieldWidget->Index]=(FieldWidget);
			UUniformGridSlot*GridSlot=	 Cast<UUniformGridSlot>(FieldWidget->Slot) ;
				if(GridSlot==nullptr)continue;
				GridSlot->SetRow(y);
				GridSlot->SetColumn(x); 
		 
	}
	
	UpdateFieldCount();
	return true;
}

void UDNABoard::GenerateBoard(int32 ClickColIndex, int32 ClickRowIndex)
{
	FieldArray.SetNum(ColumnNum * RowNum);
	const int32 FieldIndex= CalcIndex(ClickColIndex, ClickRowIndex);
	for (int32 i = 0; i < FieldArray.Num(); ++i)
	{
	 
		const bool bHasMine=UKismetMathLibrary::RandomFloat() < MineChance;
		 FieldArray[i] =  bHasMine;
	 
		WidgetsField[i]->SetIndexAndMine(i, FieldIndex==i?false:bHasMine);
	 
	}

	for (int i = 0; i < WidgetsField.Num(); ++i)
	{
		 	WidgetsField[i]->NumberMinesAround=CalculateFieldNumber(WidgetsField[i]->BoardPosition.X,WidgetsField[i]->BoardPosition.Y);
		
	}
	FieldArray[FieldIndex] = false;
	bBoardGenerated = true;
 
}


int32 UDNABoard::CalcIndex(int32 ColIndex, int32 RowIndex)
{
	return ColIndex * ColumnNum +RowIndex ;
}

int32 UDNABoard::GetValue(int32 ColIndex, int32 RowIndex)
{
	if(!CheckIsValidIndex(ColIndex,RowIndex))
		return 0;
	const int32 FieldIndex= CalcIndex(ColIndex, RowIndex); 
	return FieldArray[FieldIndex];
}

bool UDNABoard::CheckIsValidIndex(int32 ColIndex, int32 RowIndex)
{
	if (ColIndex < 0 || ColIndex >= ColumnNum || RowIndex < 0 || RowIndex >= RowNum)
	{
		return false;
	}
	return true;
}


void UDNABoard::UpdateFieldCount()
{
	{
		/* if(ensureMsgf(HiddenFieldCount==nullptr,TEXT("HiddenFieldCount is NULL, Assign it")))
			 return;*/
		if(HiddenFieldCount==nullptr)return;
		
		int32 FieldCount=0;
		for (UDNAField* Field : WidgetsField)
		{
			//this should not happen but anyway
			if(Field==nullptr)continue;
			FieldCount+=Field->FieldState.MatchesTag(GT_FIELD_HIDDEN)?1:0;
		}
		
		HiddenFieldCount->SetText(FText::AsNumber(FieldCount));
		
	}
}

void UDNABoard::ChangeGameStatus(FGameplayTag NewStatus)
{
	CurrentGameStatus=NewStatus;
	OnGameStatusChange(CurrentGameStatus);
}

void UDNABoard::ChangeDifficulty(float NewValue)
{
	if(bBoardGenerated)return;
	MineChance=NewValue;
}

void UDNABoard::ChangeSize(const int32 ColumnNumber, const int32 RowNumber)
{
	if(!CurrentGameStatus.MatchesTag(GT_GAME_STATUS_WAITING))return;
	ColumnNum=FMath::Clamp(ColumnNumber,3,12);
	RowNum=FMath::Clamp(RowNumber,3,12);
	check(GenerateFieldWidgets());
	UpdateFieldCount();
}
