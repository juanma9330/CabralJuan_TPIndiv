// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "iten.generated.h"

UCLASS()
class TPINDIVJUANCABRAL_API Aiten : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aiten();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int vida;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Fuerza;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "stats")
	float Defensa;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "stats")
	float Inteligencia;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "stats")
	int Suerte;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mago")
	int Agilidad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mago", meta = (ClampMin = "0.0", ClampMax = "75.0", UIMin ="0.0",UIMax="75.0", tooltip = "Maná del mago"))
	int Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stats")
	float Carisma;


	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0", tooltip = "Estamina del personaje"))
	int Estamina;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
};
