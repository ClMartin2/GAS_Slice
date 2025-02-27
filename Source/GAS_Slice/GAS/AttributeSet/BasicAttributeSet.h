#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BasicAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GAS_SLICE_API UBasicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet,Health);

	UPROPERTY(BlueprintReadOnly,Category="Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet,Mana);

	UPROPERTY(BlueprintReadOnly,Category="Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet,MaxMana);

	UPROPERTY(BlueprintReadOnly,Category="Attribute")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet,Speed);

	UPROPERTY(BlueprintReadOnly,Category="Attribute")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet,Shield);

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
