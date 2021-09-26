#pragma once

#include "UObject.h"
#include "Primitives.h"
#include "Containers/Array.h"
#include "EnumAsByte.h"

namespace UE4
{
	struct FStreamingLevelsToConsider
	{
		/** Priority sorted array of streaming levels actively being considered. */
		TArray<class ULevelStreaming *> StreamingLevels;

		enum class EProcessReason : uint8
		{
			Add,
			Reevaluate
		};

		/** Streaming levels that had their priority changed or were added to the container while consideration was underway. */
		TSortedMap<ULevelStreaming *, EProcessReason> LevelsToProcess;

		/** Whether the streaming levels are under active consideration or not */
		bool bStreamingLevelsBeingConsidered;
	};

	class UWorldProxy
	{
	public:
		class UWorld *World;
	};

	class UWorld : public UObject //, public FNetworkNotify
	{
	public:
		//void *Vfptr;

		class ULevel *PersistentLevel;

		/** The NAME_GameNetDriver game connection(s) for client/server communication */
		class UNetDriver *NetDriver;

		/** Line Batchers. All lines to be drawn in the world. */
		class ULineBatchComponent *LineBatcher;

		/** Persistent Line Batchers. They don't get flushed every frame.  */
		class ULineBatchComponent *PersistentLineBatcher;

		/** Foreground Line Batchers. This can't be Persistent.  */
		class ULineBatchComponent *ForegroundLineBatcher;

		/** Instance of this world's game-specific networking management */
		class AGameNetworkManager *NetworkManager;

		/** Instance of this world's game-specific physics collision handler */
		class UPhysicsCollisionHandler *PhysicsCollisionHandler;

		/** Array of any additional objects that need to be referenced by this world, to make sure they aren't GC'd */
		TArray<UObject *> ExtraReferencedObjects;

		/**
	 * External modules can have additional data associated with this UWorld.
	 * This is a list of per module world data objects. These aren't
	 * loaded/saved by default.
	 */
		TArray<UObject *> PerModuleDataObjects;
		/** Level collection. ULevels are referenced by FName (Package name) to avoid serialized references. Also contains offsets in world units */
		TArray<class ULevelStreaming *> StreamingLevels;

		/** This is the list of streaming levels that are actively being considered for what their state should be. It will be a subset of StreamingLevels */
		FStreamingLevelsToConsider StreamingLevelsToConsider;

		FString StreamingLevelsPrefix;

		class ULevel *CurrentLevelPendingVisibility;

		class ULevel *CurrentLevelPendingInvisibility;

		//	UE_DEPRECATED(4.26, "DemoNetDriver will be made private in a future release.  Please use GetDemoNetDriver/SetDemoNetDriver instead.")
		class UDemoNetDriver *DemoNetDriver;
		class AParticleEventManager *MyParticleEventManager;
		class APhysicsVolume *DefaultPhysicsVolume;
		TArray<FVector> ViewLocationsRenderedLastFrame;

		/** The current renderer feature level of this world */
		TEnumAsByte<ERHIFeatureLevel::Type> FeatureLevel;

		/** The current ticking group																								*/
		TEnumAsByte<ETickingGroup> TickGroup;

		/** The type of world this is. Describes the context in which it is being used (Editor, Game, Preview etc.) */
		TEnumAsByte<EWorldType::Type> WorldType;

		/** set for one tick after completely loading and initializing a new world
	 * (regardless of whether it's LoadMap() or seamless travel)
	 */
		uint8 bWorldWasLoadedThisTick : 1;

		/**
	 * Triggers a call to PostLoadMap() the next Tick, turns off loading movie if LoadMap() has been called.
	 */
		uint8 bTriggerPostLoadMap : 1;

		/** Whether we are in the middle of ticking actors/components or not														*/
		uint8 bInTick : 1;

		/** Whether we have already built the collision tree or not                                                                 */
		uint8 bIsBuilt : 1;

		/** We are in the middle of actor ticking, so add tasks for newly spawned actors											*/
		uint8 bTickNewlySpawned : 1;

		/** 
	 * Indicates that during world ticking we are doing the final component update of dirty components 
	 * (after PostAsyncWork and effect physics scene has run. 
	 */
		uint8 bPostTickComponentUpdate : 1;

		/** Whether world object has been initialized via Init()																	*/
		uint8 bIsWorldInitialized : 1;

		/** Is level streaming currently frozen?																					*/
		uint8 bIsLevelStreamingFrozen : 1;

		/** True we want to execute a call to UpdateCulledTriggerVolumes during Tick */
		uint8 bDoDelayedUpdateCullDistanceVolumes : 1;

		/** If true this world is in the process of running the construction script for an actor */
		uint8 bIsRunningConstructionScript : 1;

		/** If true this world will tick physics to simulate. This isn't same as having Physics Scene. 
	*  You need Physics Scene if you'd like to trace. This flag changed ticking */
		uint8 bShouldSimulatePhysics : 1;

#if !UE_BUILD_SHIPPING
		/** If TRUE, 'hidden' components will still create render proxy, so can draw info (see USceneComponent::ShouldRender) */
		uint8 bCreateRenderStateForHiddenComponentsWithCollsion : 1;
#endif // !UE_BUILD_SHIPPING

		/** frame rate is below DesiredFrameRate, so drop high detail actors */
		uint8 bDropDetail : 1;

		/** frame rate is well below DesiredFrameRate, so make LOD more aggressive */
		uint8 bAggressiveLOD : 1;

		/** That map is default map or not **/
		uint8 bIsDefaultLevel : 1;

		/** Whether it was requested that the engine bring up a loading screen and block on async loading. */
		uint8 bRequestedBlockOnAsyncLoading : 1;

		/** Whether actors have been initialized for play */
		uint8 bActorsInitialized : 1;

		/** Whether BeginPlay has been called on actors */
		uint8 bBegunPlay : 1;

		/** Whether the match has been started */
		uint8 bMatchStarted : 1;

		/**  When ticking the world, only update players. */
		uint8 bPlayersOnly : 1;

		/** Indicates that at the end the frame bPlayersOnly will be set to true. */
		uint8 bPlayersOnlyPending : 1;

		/** Is the world in its actor initialization phase. */
		uint8 bStartup : 1;

		/** Is the world being torn down */
		uint8 bIsTearingDown : 1;

		/**
	 * This is a bool that indicates that one or more blueprints in the level (blueprint instances, level script, etc)
	 * have compile errors that could not be automatically resolved.
	 */
		uint8 bKismetScriptError : 1;

		// Kismet debugging flags - they can be only editor only, but they're uint32, so it doens't make much difference
		uint8 bDebugPauseExecution : 1;

		/** When set, camera is potentially moveable even when paused */
		uint8 bIsCameraMoveableWhenPaused : 1;

		/** Indicates this scene always allows audio playback. */
		uint8 bAllowAudioPlayback : 1;

		/** Keeps track whether actors moved via PostEditMove and therefore constraint syncup should be performed. */
		uint8 bAreConstraintsDirty : 1;

	private:
		/** Whether the render scene for this World should be created with HitProxies or not */
		uint8 bRequiresHitProxies : 1;

		/** Whether to do any ticking at all for this world. */
		uint8 bShouldTick : 1;

		/** Whether we have a pending call to BuildStreamingData(). */
		uint8 bStreamingDataDirty : 1;

		/** Is forcibly unloading streaming levels?																					*/
		uint8 bShouldForceUnloadStreamingLevels : 1;

		/** Is forcibly making streaming levels visible?																			*/
		uint8 bShouldForceVisibleStreamingLevels : 1;

		/** Is there at least one material parameter collection instance waiting for a deferred update?								*/
		uint8 bMaterialParameterCollectionInstanceNeedsDeferredUpdate : 1;

		/** The world's navigation data manager */
		class UNavigationSystemBase *NavigationSystem;

		/** The current GameMode, valid only on the server */
		class AGameModeBase *AuthorityGameMode;

		/** The replicated actor which contains game state information that can be accessible to clients. Direct access is not allowed, use GetGameState<>() */
		class AGameStateBase *GameState;

		/** The AI System handles generating pathing information and AI behavior */
		class UAISystemBase *AISystem;

		/** RVO avoidance manager used by game */
		class UAvoidanceManager *AvoidanceManager;

		/** Array of levels currently in this world. Not serialized to disk to avoid hard references. */
		TArray<class ULevel *> Levels;

		/** Array of level collections currently in this world. */
		TArray<FLevelCollection> LevelCollections;

		/** Index of the level collection that's currently ticking. */
		int32 ActiveLevelCollectionIndex;

	public:
		/** Handle to the active audio device for this world. */
		FAudioDeviceHandle AudioDeviceHandle;

		/** Handle to delegate in case audio device is destroyed. */
		FDelegateHandle AudioDeviceDestroyedHandle;

		class UGameInstance *OwningGameInstance;

		/** Parameter collection instances that hold parameter overrides for this world. */
		TArray<class UMaterialParameterCollectionInstance *> ParameterCollectionInstances;

		/** 
	 * Canvas object used for drawing to render targets from blueprint functions eg DrawMaterialToRenderTarget.
	 * This is cached as UCanvas creation takes >100ms.
	 */
		class UCanvas *CanvasForRenderingToTarget;

		class UCanvas *CanvasForDrawMaterialToRenderTarget;

	public:
		/** The interface to the scene manager for this world. */
		class FSceneInterface *Scene;

		/** A static map that is populated before loading a world from a package. This is so UWorld can look up its WorldType in ::PostLoad */
		static TMap<FName, EWorldType::Type> WorldTypePreLoadMap;

		/** List of all the controllers in the world. */
		TArray<TWeakObjectPtr<class AController>> ControllerList;

		/** List of all the player controllers in the world. */
		TArray<TWeakObjectPtr<class APlayerController>> PlayerControllerList;

		/** List of all the cameras in the world that auto-activate for players. */
		TArray<TWeakObjectPtr<ACameraActor>> AutoCameraActorList;

		/** List of all physics volumes in the world. Does not include the DefaultPhysicsVolume. */
		TArray<TWeakObjectPtr<APhysicsVolume>> NonDefaultPhysicsVolumeList;

		/** Physics scene for this world. */
		FPhysScene *PhysicsScene;
		// Note that this should be merged with PhysScene going forward but is needed for now.
	public:
#if INCLUDE_CHAOS
		/** Current global physics scene. */
		TSharedPtr<FPhysScene_Chaos> PhysicsScene_Chaos;

		/** Default global physics scene. */
		TSharedPtr<FPhysScene_Chaos> DefaultPhysicsScene_Chaos;
#endif
	private:
		/** Array of components that need to wait on tasks before end of frame updates */
		TSet<UActorComponent *> ComponentsThatNeedPreEndOfFrameSync;

		/** Array of components that need updates at the end of the frame */
		TArray<UActorComponent *> ComponentsThatNeedEndOfFrameUpdate;

		/** Array of components that need game thread updates at the end of the frame */
		TArray<UActorComponent *> ComponentsThatNeedEndOfFrameUpdate_OnGameThread;

		/** The state of async tracing - abstracted into its own object for easier reference */
		FWorldAsyncTraceState AsyncTraceState;

#if WITH_EDITOR
		/**	Objects currently being debugged in Kismet	*/
		FBlueprintToDebuggedObjectMap BlueprintObjectsBeingDebugged;
#endif

		/** a delegate that broadcasts a notification whenever an actor is spawned */
		FOnActorSpawned OnActorSpawned;

		/** a delegate that broadcasts a notification before a newly spawned actor is initialized */
		FOnActorSpawned OnActorPreSpawnInitialization;

		/** Reset Async Trace Buffer **/
		void ResetAsyncTrace();

		/** Wait for all Async Trace Buffer to be done **/
		void WaitForAllAsyncTraceTasks();

		/** Finish Async Trace Buffer **/
		void FinishAsyncTrace();

		/** Utility function that is used to ensure that a World has the correct WorldSettings */
		void RepairWorldSettings();

		/** Utility function to cleanup streaming levels that point to invalid level packages */
		void RepairStreamingLevels();

#if INCLUDE_CHAOS
		/** Utility function that is used to ensure that a World has the correct ChaosActor */
		void RepairChaosActors();
#endif

		/** Gameplay timers. */
		class FTimerManager *TimerManager;

		/** Latent action manager. */
		struct FLatentActionManager LatentActionManager;

		/** Timestamp (in FPlatformTime::Seconds) when the next call to BuildStreamingData() should be made, if bDirtyStreamingData is true. */
		double BuildStreamingDataTimer;

		//DECLARE_EVENT_OneParam(UWorld, FOnNetTickEvent, float);
		//DECLARE_EVENT(UWorld, FOnTickFlushEvent);
		/** Event to gather up all net drivers and call TickDispatch at once */
		FOnNetTickEvent TickDispatchEvent;

		/** Event to gather up all net drivers and call PostTickDispatch at once */
		FOnTickFlushEvent PostTickDispatchEvent;

		/** Event to gather up all net drivers and call TickFlush at once */
		FOnNetTickEvent TickFlushEvent;

		/** Event to gather up all net drivers and call PostTickFlush at once */
		FOnTickFlushEvent PostTickFlushEvent;

		/** Called when the number of levels changes. */
		//	DECLARE_EVENT(UWorld, FOnLevelsChangedEvent);

		/** Broadcasts whenever the number of levels changes */
		FOnLevelsChangedEvent LevelsChangedEvent;

		//	DECLARE_EVENT(UWorld, FOnBeginTearingDownEvent);

		/** Broadcasted on UWorld::BeginTearingDown */
		FOnBeginTearingDownEvent BeginTearingDownEvent;

		FOnMovieSceneSequenceTick MovieSceneSequenceTick;

	public:
		/** The URL that was used when loading this World.																			*/
		FURL URL;

		/** Interface to the FX system managing particles and related effects for this world.										*/
		class FFXSystemInterface *FXSystem;

		/** Data structures for holding the tick functions that are associated with the world (line batcher, etc) **/
		class FTickTaskLevel *TickTaskLevel;

		/** Tick function for starting physics																						*/
		FStartPhysicsTickFunction StartPhysicsTickFunction;
		/** Tick function for ending physics																						*/
		FEndPhysicsTickFunction EndPhysicsTickFunction;

		/** Counter for allocating game- unique controller player numbers															*/
		int32 PlayerNum;

		/** Number of frames to delay Streaming Volume updating, useful if you preload a bunch of levels but the camera hasn't caught up yet (INDEX_NONE for infinite) */
		int32 StreamingVolumeUpdateDelay;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		/** When non-'None', all line traces where the TraceTag match this will be drawn */
		FName DebugDrawTraceTag;

		/** When set to true, all scene queries will be drawn */
		bool bDebugDrawAllTraceTags;
#endif

		/** Called when the world computes how post process volumes contribute to the scene. */
		DECLARE_EVENT_TwoParams(UWorld, FOnBeginPostProcessSettings, FVector, FSceneView *);
		FOnBeginPostProcessSettings OnBeginPostProcessSettings;

		/** An array of post processing volumes, sorted in ascending order of priority.					*/
		TArray<IInterface_PostProcessVolume *> PostProcessVolumes;

		/** Set of AudioVolumes sorted by  */
		// TODO: Make this be property UPROPERTY(Transient)
		TArray<class AAudioVolume *> AudioVolumes;

		/** Time in FPlatformTime::Seconds unbuilt time was last encountered. 0 means not yet.							*/
		double LastTimeUnbuiltLightingWasEncountered;

		/**  Time in seconds since level began play, but IS paused when the game is paused, and IS dilated/clamped. */
		float TimeSeconds;

		/**  Time in seconds since level began play, but IS NOT paused when the game is paused, and IS dilated/clamped. */
		float UnpausedTimeSeconds;

		/** Time in seconds since level began play, but IS NOT paused when the game is paused, and IS NOT dilated/clamped. */
		float RealTimeSeconds;

		/** Time in seconds since level began play, but IS paused when the game is paused, and IS NOT dilated/clamped. */
		float AudioTimeSeconds;

		/** Frame delta time in seconds adjusted by e.g. time dilation. */
		float DeltaTimeSeconds;

		/** time at which to start pause **/
		float PauseDelay;

		/** Current location of this world origin */
		FIntVector OriginLocation;

		/** Requested new world origin location */
		FIntVector RequestedOriginLocation;

		/** World origin offset value. Non-zero only for a single frame when origin is rebased */
		FVector OriginOffsetThisFrame;

		/** Amount of time to wait before traveling to next map, gives clients time to receive final RPCs @see ServerTravelPause */
		float NextSwitchCountdown;

		/** All levels information from which our world is composed */
		class UWorldComposition *WorldComposition;

		/** Whether we flushing level streaming state */
		EFlushLevelStreamingType FlushLevelStreamingType;

		/** The type of travel to perform next when doing a server travel */
		TEnumAsByte<ETravelType> NextTravelType;

	private:
		/** An internal count of how many streaming levels are currently loading */
		uint16 NumStreamingLevelsBeingLoaded; // Move this somewhere better

		friend struct FWorldNotifyStreamingLevelLoading;

	public:
		/** The URL to be used for the upcoming server travel */
		FString NextURL;

		/** array of levels that were loaded into this map via PrepareMapChange() / CommitMapChange() (to inform newly joining clients) */
		TArray<FName> PreparingLevelNames;

		/** Name of persistent level if we've loaded levels via CommitMapChange() that aren't normally in the StreamingLevels array (to inform newly joining clients) */
		FName CommittedPersistentLevelName;

#if !UE_BUILD_SHIPPING
		/**
	 * This is a int on the level which is set when a light that needs to have lighting rebuilt
	 * is moved.  This is then checked in CheckMap for errors to let you know that this level should
	 * have lighting rebuilt.
	 **/
		uint32 NumLightingUnbuiltObjects;

		uint32 NumUnbuiltReflectionCaptures;

		/** Num of components missing valid texture streaming data. Updated in map check. */
		int32 NumTextureStreamingUnbuiltComponents;

		/** Num of resources that have changed since the last texture streaming build. Updated in map check. */
		int32 NumTextureStreamingDirtyResources;
#endif

		/** Indicates that the world has marked contained objects as pending kill */
		bool HasMarkedObjectsPendingKill() const { return bMarkedObjectsPendingKill; }

	private:
		uint32 bMarkedObjectsPendingKill : 1;

		uint32 CleanupWorldTag;
		static uint32 CleanupWorldGlobalTag;

	public:
		//Experimental: In game performance tracking.
		FWorldInGamePerformanceTrackers *PerfTrackers;
	};
}