// Copyright 2021 DNABLOCK. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DNABoard.generated.h"

class UWrapBox;


/**
 * 
 */
UCLASS()
class DNAMINESWEEPER_API UDNABoard : public UUserWidget
{
	GENERATED_BODY()

	//UUserWidget interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

public:

	UFUNCTION(BlueprintCallable)
	void HandleClickOnField(int32 ColIndex, int32 RowIndex);

	/** 
	 *  -1: Mine
	 *   X: Field next to X mine
	 */
	UFUNCTION(BlueprintCallable)
	int32 CalculateFieldNumber(int32 ColIndex, int32 RowIndex);

protected:

	UFUNCTION(BlueprintImplementableEvent)
	UUserWidget* CreateFieldWidget(int32 ColIndex, int32 RowIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void RevealField(int32 ChildIndex, int32 FieldContent);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleGameOver();

	bool GenerateFieldWidgets();

	void GenerateBoard(int32 ClickColIndex, int32 ClickRowIndex);

	UFUNCTION(BlueprintPure)
	int32 CalcIndex(int32 ColIndex, int32 RowIndex);

	UFUNCTION(BlueprintPure)
	int32 GetValue(int32 ColIndex, int32 RowIndex);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = ">Widgets")
	UWrapBox* Fields;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ColumnNum = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RowNum = 12;

	UPROPERTY(EditAnywhere)
	float MineChance = 0.1;


	bool bBoardGenerated = false;

	// TODO: comment
	TArray<bool> FieldArray;
};
