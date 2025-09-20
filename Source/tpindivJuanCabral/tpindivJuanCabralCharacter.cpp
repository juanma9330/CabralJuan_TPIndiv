// Copyright Epic Games, Inc. All Rights Reserved.
//Incluye el header del personaje, donde se declaran las variables, componentes y funciones
#include "tpindivJuanCabralCharacter.h"
//Permite acceder a información del jugador local, como el control o imputs
#include "Engine/LocalPlayer.h"
//incluye la definición del componente de la cámara, para crearla y manipularla
#include "Camera/CameraComponent.h"
//Incluye la capsula del personaje, que funciona como el collider principal
#include "Components/CapsuleComponent.h"
//maneja toda la lógica de movimiento del personaje:
#include "GameFramework/CharacterMovementComponent.h"
//maneja el brazo de la cámara el SpringArm
#include "GameFramework/SpringArmComponent.h"
//control necesario para obtener input y rotación del personaje
#include "GameFramework/Controller.h"
//bindea imputs a acciones
#include "EnhancedInputComponent.h"
//subsistema que gestiona el imput mapping Context
#include "EnhancedInputSubsystems.h"
//contenedor de los valores de entrada
#include "InputActionValue.h"
//incluye el header del proyecto para obtener las definiciones necesarias del juego
#include "tpindivJuanCabral.h"

//se cambiaron todos los parámetros .f para probar 
// establece la clase base de la cual van a herecdar los child
AtpindivJuanCabralCharacter::AtpindivJuanCabralCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(65.f, 72.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1200.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 820.f;
	GetCharacterMovement()->AirControl = 0.15f;
	GetCharacterMovement()->MaxWalkSpeed = 740.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 200.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	//configuran el SpringArm de la camára

	//crea el SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//se adjunta al RootComponent del personaje
	CameraBoom->SetupAttachment(RootComponent);
	//maneja la distancia
	CameraBoom->TargetArmLength = 650.0f;
	//para tener control de giro de la cámara
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	//configuracion de la camara, la crea, asigna como su root al sprinarm y evita que rote la camara junto al personaje
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}
//configura las acciones de input del jugador, asignamos la acción correspondiente segun la tecla de input
void AtpindivJuanCabralCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	//comprueba el sistema de entrada, el Enhanced Input System
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		//funacion de salto, empieza cuando apreto y termina cuando suelto la tecla
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// nos movemos continuamente mientras apretemos la tecla
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AtpindivJuanCabralCharacter::Move);
		// conecta el movimiento del mouse con la rotación de la cámara en el juego,
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AtpindivJuanCabralCharacter::Look);

		// Looking
		// bindea la acción de mirar a la función de código que maneja la rotación de la cámara
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AtpindivJuanCabralCharacter::Look);
	
		//dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AtpindivJuanCabralCharacter::dashing);

		//bindeamos el inicio y fin de la acción correr segun empezamos y soltamos la tecla
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AtpindivJuanCabralCharacter::StartCorrer);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AtpindivJuanCabralCharacter::Endcorrer);
		// mostrar un mensaje cuando apretamos la tecla
		EnhancedInputComponent->BindAction(MostrarMensajeAction, ETriggerEvent::Triggered, this, &AtpindivJuanCabralCharacter::MostrarMensaje);

	}

	else
	{
	//cuando hay un error al encontrar el enhanced imput component muestra ese mensaje de error del text

	UE_LOG(LogtpindivJuanCabral, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//maneja el movimiento del personaje en base a los valores de entrada
void AtpindivJuanCabralCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	//Calcula el vector de dirección de movimiento segun la entrada
	FVector2D MovementVector = Value.Get<FVector2D>();
	//toma el vector y lo traduce en movimiento
	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

//maneja la rotación de la cámara del personaje en base a valores de entrada
void AtpindivJuanCabralCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	//extrae la entrada del jugador para la rotación de la cámara y la almacena en un vector 2D.
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	//toma el vector para establecer a donde miramos
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}
//Funcion implementada correr, aumenta la velocidad de caminar en StartCorrer y vuelve a la velocidad inicial en el Endcorrer
void AtpindivJuanCabralCharacter::StartCorrer()
{
	GetCharacterMovement()->MaxWalkSpeed = 1800.f;

}

void AtpindivJuanCabralCharacter::Endcorrer()
{
	GetCharacterMovement()->MaxWalkSpeed = 740.f;

}

void AtpindivJuanCabralCharacter::dashing()
{
	// Obtener la dirección del movimiento actual del personaje y si no se mueve hacer el dash hacia donde está mirando
	FVector DashDirection = GetVelocity().GetSafeNormal();
	if (DashDirection.IsNearlyZero())
	{
		DashDirection = GetActorForwardVector();
	}

	// Llamo a la función LaunchCharacter en el personaje.
	LaunchCharacter(DashDirection * 15000.f, true, true);
}
//el contenido de la función mostrar mensaje con la duración en pantalla, color y mensaje
void AtpindivJuanCabralCharacter::MostrarMensaje()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hola Kent, Mejorate pronto!!")));
	}
}

//Estas funciones Do se usan para el control tactil de inputs 
//función de movimiento de personaje
void AtpindivJuanCabralCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		//Obtiene la rotación del controller, es decir hacia dónde mira el personaje
		// find out which way is forward
		//YawRotation se usa para movernos en el plano horizontal utilizando los ejes X e Y

		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		//Calcula la dirección hacia adelante
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//calcula la dirección hacia la derecha del jugador.
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		//aplica el movimiento al Character
		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
//implementacion de la función mirar
void AtpindivJuanCabralCharacter::DoLook(float Yaw, float Pitch)
{
	//Comprueba que el personaje tiene un controlador asignado
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		//Aplica rotación horizontal al controller del personaje, modificando el Yaw de la cámara.
		AddControllerYawInput(Yaw);
		//Aplica rotación vertical al controller que modifica el Pitch de la cámara.
		AddControllerPitchInput(Pitch);
	}
}
//funciones callback para manejar el salto y el final de la accion de salto
void AtpindivJuanCabralCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AtpindivJuanCabralCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
