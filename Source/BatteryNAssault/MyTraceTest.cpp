// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyTraceTest.h"


AMyTraceTest::AMyTraceTest()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.3f;
	AmmoUsed = 1;
	Range = 500.0f;
	DamageDealt= 5.0f;



}




