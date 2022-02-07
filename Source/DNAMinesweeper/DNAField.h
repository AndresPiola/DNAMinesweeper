// Copyright 2021 DNABLOCK. All rights reserved.

#pragma once

#include "CoreMinimal.h" 
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DNAField.generated.h"

class UDNABoard;
UCLASS()
class DNAMINESWEEPER_API UDNAField : public UUserWidget
{
	GENERATED_BODY()

	  
public:
	UPROPERTY(BlueprintReadWrite,Category="DNAField | Properties")
	FGameplayTag FieldState;
	
	UPROPERTY(BlueprintReadOnly)
	FIntPoint BoardPosition;

	UPROPERTY(BlueprintReadOnly)
	int32 Index;

	UPROPERTY(BlueprintReadOnly)
	int32 NumberMinesAround=0;

	UPROPERTY(BlueprintReadOnly)
	bool bHasMine;

	UPROPERTY(BlueprintReadOnly)
	UDNABoard* BoardReference;

 	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DNAField | Widgets")
 	UImage* FlagIcon;

 

protected:
	
  
	UFUNCTION()
	void MouseClickBoardCallback(bool bWasLeftMouseClicked);


	UFUNCTION()
	void RevealAllCallback();

	UFUNCTION()
	void SwitchFlagState();

public:
	UFUNCTION()
	void RevealFieldCallback(int32 IndexFromBoard,int32 Result);

	bool RevealField(int32 IndexFromBoard,int32 Result);

	void InitializeField(FIntPoint NewBoardPosition,UDNABoard* Board);

	void SetIndexAndMine(int32 FieldIndex,bool bSetAsMined);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRevealContent(int32 Content);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRevealAll();
	
};
