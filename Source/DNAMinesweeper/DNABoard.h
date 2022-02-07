// Copyright 2021 DNABLOCK. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DNAField.h"
#include "Blueprint/UserWidget.h" 
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "DNABoard.generated.h"

class UTextBlock;
class UDNAField; 

#define GT_FIELD_HIDDEN FGameplayTag::RequestGameplayTag(FName("Field.State.Hidden"))
#define GT_FIELD_REVEALED FGameplayTag::RequestGameplayTag(FName("Field.State.Revealed"))
#define GT_FIELD_FLAGGED FGameplayTag::RequestGameplayTag(FName("Field.State.Flagged"))

#define GT_GAME_STATUS_WAITING FGameplayTag::RequestGameplayTag(FName("GameState.Waiting"))
#define GT_GAME_STATUS_BEGIN FGameplayTag::RequestGameplayTag(FName("GameState.Begin"))
#define GT_GAME_STATUS_GAME_OVER FGameplayTag::RequestGameplayTag(FName("GameState.GameOver"))

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyGameStateChange,FGameplayTag,CurrentState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyMouseClick,bool,bLeftMouseClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyReveal,int32,Index,int32,RevealResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyRevealAll);

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

 
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:


		
	UPROPERTY(BlueprintReadOnly,Category="DNA UI")
	bool bIsLeftMouseClicked;

	UPROPERTY(BlueprintReadOnly,Category="DNA UI")
	FGameplayTag CurrentGameStatus;
	
	
	
	UFUNCTION(BlueprintCallable)
	void HandleClickOnField(int32 ColIndex, int32 RowIndex,int32 FieldIndex);

	/** 
	 *  -1: Mine
	 *   X: Field next to X mine
	 */
	UFUNCTION(BlueprintCallable)
	int32 CalculateFieldNumber(int32 ColIndex, int32 RowIndex);

	void LookAround(int32 ColIndex, int32 RowIndex);
	

	UPROPERTY(BlueprintAssignable)
	FNotifyMouseClick OnMouseClickBoard;

	UPROPERTY(BlueprintAssignable)
	FNotifyReveal OnRevealField;

	UPROPERTY(BlueprintAssignable)
	FNotifyRevealAll OnRevealAll;

	UPROPERTY(BlueprintAssignable)
	FNotifyRevealAll OnGameStatusChanged;

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

	bool CheckIsValidIndex(int32 ColIndex, int32 RowIndex);
public:
	
	void UpdateFieldCount();

	UFUNCTION(BlueprintCallable)
void ChangeGameStatus(FGameplayTag NewStatus);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStatusChange(FGameplayTag NewStatus);
	
	UFUNCTION(BlueprintCallable)
	void ChangeDifficulty(float NewValue);
	
	UFUNCTION(BlueprintCallable)
	void ChangeSize(int32 ColumnNumber,int32 RowNumber);
	
protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = ">Widgets")
	UUniformGridPanel* Fields;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDNAField> FieldClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DNAField | Widgets")
	UTextBlock* HiddenFieldCount;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ColumnNum = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RowNum = 12;

	
	UPROPERTY()
	bool bBoardGenerated = false;

	// TODO: comment use uproperty or this could be  garbage collected
	UPROPERTY()
	TArray<bool> FieldArray;

	UPROPERTY()
	TArray<UDNAField*> WidgetsField;

	UPROPERTY()
	TArray<int32> CheckList;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MineChance = 0.1;


};
