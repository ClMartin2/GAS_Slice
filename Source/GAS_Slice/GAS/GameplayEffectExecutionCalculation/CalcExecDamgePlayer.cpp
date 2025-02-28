// Fill out your copyright notice in the Description page of Project Settings.


#include "../../GAS/GameplayEffectExecutionCalculation/CalcExecDamgePlayer.h"

#include "GAS_Slice/GAS/AttributeSet/BasicAttributeSet.h"

struct DamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet,Health,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet,Shield,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet,Damage,Target,false);
	}
};

static DamageCapture& GetDamageCapture()
{
	static DamageCapture DamageCapture;
	return DamageCapture;
}

UCalcExecDamgePlayer::UCalcExecDamgePlayer()
{
	RelevantAttributesToCapture.Add(GetDamageCapture().HealthDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().ShieldDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().DamageDef);
}

void UCalcExecDamgePlayer::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	// AActor* TargetActor = TargetAbilitySystemComponent? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	//
	// UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	// AActor* SourceActor = SourceAbilitySystemComponent? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	//
	// const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	//
	// const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	// const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	//
	// FAggregatorEvaluateParameters EvalutionParameters;
	// EvalutionParameters.SourceTags = SourceTags;
	// EvalutionParameters.TargetTags = TargetTags;
	//
	// float Health = 0;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().HealthDef,EvalutionParameters,Health);
	//
	// float Shield = 0;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ShieldDef,EvalutionParameters,Shield);
	//
	// float Damage = 0;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().DamageDef,EvalutionParameters,Damage);
	//
	// float DifferenceShieldDamage = Damage - Shield;
	//
	// float HealthToAdd = 0;
	// float ShieldToAdd = 0;
	//
	// if (DifferenceShieldDamage < 0)
	// {
	// 	ShieldToAdd = -Damage;
	// }else
	// {
	// 	Damage = DifferenceShieldDamage;
	// 	ShieldToAdd = -Shield;
	// 	HealthToAdd -= Damage;
	// }
	//
	// OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().HealthProperty, EGameplayModOp::Additive,HealthToAdd));
	// OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().ShieldProperty, EGameplayModOp::AddBase,ShieldToAdd));
}

