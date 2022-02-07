// Copyright 2021 DNABLOCK. All rights reserved.


#include "DNABoard.h"

#include "Components/WrapBox.h"
#include "Kismet/KismetMathLibrary.h"



void UDNABoard::NativePreConstruct()
{
	check(GenerateFieldWidgets());
}


void UDNABoard::HandleClickOnField(int32 ColIndex, int32 RowIndex)
{
	if (!bBoardGenerated)
	{
		GenerateBoard(ColIndex, RowIndex);
	}

	const int32 Index = CalcIndex(ColIndex, RowIndex);
	
	if (FieldArray[Index])
	{
		HandleGameOver();
	}
	else
	{
		RevealField(Index, CalculateFieldNumber(ColIndex, RowIndex));
	}
}

int32 UDNABoard::CalculateFieldNumber(int32 ColIndex, int32 RowIndex)
{
	if (FieldArray[CalcIndex(ColIndex, RowIndex)])
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

bool UDNABoard::GenerateFieldWidgets()
{
	if (RowNum < 0 || ColumnNum < 0)
	{
		return false;
	}

	Fields->ClearChildren();

	for (int32 j = 0; j < RowNum; ++j)
	{
		for (int32 i = 0; i < ColumnNum; ++i)
		{
			UUserWidget* FieldWidget = CreateFieldWidget(i, j);
			if (Fields != nullptr)
			{
				Fields->AddChild(FieldWidget);
			}
		}
	}

	return true;
}

void UDNABoard::GenerateBoard(int32 ClickColIndex, int32 ClickRowIndex)
{
	FieldArray.SetNum(ColumnNum * RowNum);

	for (int32 i = 0; i < FieldArray.Num(); ++i)
	{
		const float RandomValue = UKismetMathLibrary::RandomFloat();
		FieldArray[i] = RandomValue < MineChance;
	}

	FieldArray[CalcIndex(ClickColIndex, ClickRowIndex)] = false;
	bBoardGenerated = true;
}


int32 UDNABoard::CalcIndex(int32 ColIndex, int32 RowIndex)
{
	return RowIndex * ColumnNum + ColIndex;
}

int32 UDNABoard::GetValue(int32 ColIndex, int32 RowIndex)
{
	if (ColIndex < 0 || ColIndex >= ColumnNum || RowIndex < 0 || RowIndex >= RowNum)
	{
		return 0;
	}

	return FieldArray[CalcIndex(ColIndex, RowIndex)];
}
