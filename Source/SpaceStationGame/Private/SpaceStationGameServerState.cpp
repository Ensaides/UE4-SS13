// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "MySQLObject.h"
#include "OpenCLObject.h"
#include <map>
#include <unordered_map>
#include "RecipeContainer.h"
#include "SpaceStationGameServerState.h"

#define RECIPES_MAX 2048

// Sets default values
ASpaceStationGameServerState::ASpaceStationGameServerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	RecipeContainer = ObjectInitializer.CreateDefaultSubobject<URecipeContainer>(this, TEXT("Recipe Container"), false);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		RecipeLookupDataTable_BP(TEXT("DataTable'/Game/Data/Recipes.Recipes'"));
	RecipeLookupTable = RecipeLookupDataTable_BP.Object;

	SetupRecipes();
}

void ASpaceStationGameServerState::BeginPlay()
{
	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		MySQLObject = NewObject<UMySQLObject>(this);

		MySQLObject->Initialize();

		//OpenCLObject = NewObject<UOpenCLObject>(this);
	}

	Super::BeginPlay();
}

void ASpaceStationGameServerState::SetUpMySQLPlayerData(APlayerController* NewPlayer)
{
	FString SteamID = NewPlayer->PlayerState->UniqueId->ToString();

	MySQLObject->GetPlayerData(SteamID, Cast<ASpaceStationGamePlayerController>(NewPlayer));
}

bool ASpaceStationGameServerState::GetPlayerBanStatus(FString Address, FString UniqueId)
{
	if (bBansLoaded)
	{

	}
}

void ASpaceStationGameServerState::SetupRecipes()
{
	static const FString ContextString(TEXT("GENERAL"));

	if (RecipeLookupTable)
	{
		for (uint32 i = 1; i <= RECIPES_MAX; i++)
		{
			FRecipeLookupTable* ROLookupRow = RecipeLookupTable->FindRow<FRecipeLookupTable>(*FString::Printf(TEXT("%d"), i), ContextString);

			std::map<EReagents, uint32> ReagentsMap;
			std::map<EReagents, uint32> ProductsMap;

			if (ROLookupRow)
			{
				{
					FString Reagents = *ROLookupRow->Reagents;

					TArray<FString> SemicolonParsed;
					Reagents.ParseIntoArray(SemicolonParsed, TEXT(";"), false);

					for (auto Iter(SemicolonParsed.CreateIterator()); Iter; Iter++)
					{
						TArray<FString> PeriodParsed;
						FString SemicolonParsedString = *Iter;

						SemicolonParsedString.ParseIntoArray(PeriodParsed, TEXT("."), true);

						if (PeriodParsed.IsValidIndex(0) && PeriodParsed.IsValidIndex(1))
						{
							std::pair<EReagents, uint32> NewReagents = GetReagentPair(PeriodParsed[0], PeriodParsed[1]);

							ReagentsMap.insert(NewReagents);
						}
					}
				}

				{
					FString Products = *ROLookupRow->Products;

					TArray<FString> SemicolonParsed;
					Products.ParseIntoArray(SemicolonParsed, TEXT(";"), false);

					for (auto Iter(SemicolonParsed.CreateIterator()); Iter; Iter++)
					{
						TArray<FString> PeriodParsed;
						FString SemicolonParsedString = *Iter;

						SemicolonParsedString.ParseIntoArray(PeriodParsed, TEXT("."), true);

						if (PeriodParsed.IsValidIndex(0) && PeriodParsed.IsValidIndex(1))
						{
							std::pair<EReagents, uint32> NewProducts = GetReagentPair(PeriodParsed[0], PeriodParsed[1]);

							ProductsMap.insert(NewProducts);
						}
					}
				}

				RecipeContainer->ConstructRecipe(ReagentsMap, ProductsMap);
			}
			else
			{
				return;
			}
		}
	}
}

std::pair<EReagents, uint32>  ASpaceStationGameServerState::GetReagentPair(FString Reagent, FString StringToInt)
{
	if (Reagent == TEXT("EChem_Explosion"))
	{
		return std::make_pair(EReagents::EChem_Explosion, FCString::Atoi(*StringToInt));
	}
	else if (Reagent == TEXT("EChem_Water"))
	{
		return std::make_pair(EReagents::EChem_Water, FCString::Atoi(*StringToInt));
	}
	else if (Reagent == TEXT("EChem_Nitrogen"))
	{
		return std::make_pair(EReagents::EChem_Nitrogen, FCString::Atoi(*StringToInt));
	}
	else if (Reagent == TEXT("EChem_Potassium"))
	{
		return std::make_pair(EReagents::EChem_Potassium, FCString::Atoi(*StringToInt));
	}
	else if (Reagent == TEXT("EChem_Silicon"))
	{
		return std::make_pair(EReagents::EChem_Silicon, FCString::Atoi(*StringToInt));
	}
	else if (Reagent == TEXT("EChem_Carbon"))
	{
		return std::make_pair(EReagents::EChem_Carbon, FCString::Atoi(*StringToInt));
	}
	else if (Reagent == TEXT("EChem_Hydrogen"))
	{
		return std::make_pair(EReagents::EChem_Hydrogen, FCString::Atoi(*StringToInt));
	}
	else return std::make_pair(EReagents::EChem_Null, FCString::Atoi(*StringToInt));
}

bool ASpaceStationGameServerState::GetRecipeExists(FRecipe InRecipe)
{
	if (RecipeContainer->IsValidLowLevel())
	{
		std::map<FRecipe, FRecipe> RecipeMap = RecipeContainer->GetRecipeMap();

		//oh god i bet this is going to be slow

		//Iterate through the whole recipe map
		for (auto iter = RecipeMap.begin(); iter != RecipeMap.end(); ++iter)
		{
			uint8 Index = 0;
			uint8 GoodIndex = 0;

			//Iterate through all the ingredients in each recipe in the recipe map
			for (auto recipeiter = iter->first.Ingredients.begin(); recipeiter != iter->first.Ingredients.end(); ++recipeiter)
			{

				Index++;

				// if the ingredient exists in the recipe map and theres more than the minimum ratio of ingredients
				if (InRecipe.Ingredients.count(recipeiter->first) == 1 && InRecipe.Ingredients[recipeiter->first] / recipeiter->second > 0)
				{
					GoodIndex++;
				}
				else
				{
					break;
				}

				if (recipeiter == --iter->first.Ingredients.end()) //&& GoodIndex == Index)
				{
					return true;
				}
			}

			//iterated through whole table, no bueno
			if (iter == RecipeMap.end())
			{
				return false;
			}

		}
		return false;
	}
	else
	{
		return false;
	}
}


//PLEASE use GetRecipeExists() before using this function you fucking chode
FRecipe ASpaceStationGameServerState::GetProductReagentFromRecipe(FRecipe InRecipe)
{
	// This function is a huge clusterfuck of iterators, please reimplement this NICK
	std::map<FRecipe, FRecipe> RecipeMap = RecipeContainer->GetRecipeMap();

	//Iterate through all the RECIPES in the RECIPE MAP
	for (auto Iter = RecipeMap.begin(); Iter != RecipeMap.end(); ++Iter)
	{
		uint8 Index = 0; //Number of ingredients in each recipe
		uint8 GoodIndex = 0; // Number of MATCHING ingredients in each recipe

		FRecipe BufferRecipe;
		std::pair<FRecipe, FRecipe> GoodRecipe;

		//Iterate through all the INGREDIENTS in the RECIPE
		for (auto RecipeIter = Iter->first.Ingredients.begin(); RecipeIter != Iter->first.Ingredients.end(); ++RecipeIter)
		{
			Index++;

			// if the ingredient exists in the recipe map and theres more than the minimum ratio of ingredients
			if (InRecipe.Ingredients.count(RecipeIter->first) == 1 && InRecipe.Ingredients[RecipeIter->first] / RecipeIter->second > 0)
			{
				BufferRecipe.Ingredients.insert(std::make_pair(RecipeIter->first, InRecipe.Ingredients[RecipeIter->first] / RecipeIter->second));

				GoodIndex++;
			}
			// If this recipe is no bueno, go to the next one!
			else break;

			//If we have reached the end of the ingredients in the recipe and the amount of ingredients match in both (GoodIndex = Index)
			if (RecipeIter == --Iter->first.Ingredients.end() && GoodIndex == Index)
			{
				TArray<uint32> IntegerBuffer;
				uint32 SmallestValue = 4294967295; //the maximum amount you can give a 32bit uint

				GoodRecipe = *Iter;

				//SO MANY ITERATORS
				for (auto BufferRecipeIter = BufferRecipe.Ingredients.begin(); BufferRecipeIter != BufferRecipe.Ingredients.end(); ++BufferRecipeIter)
				{
					IntegerBuffer.Add(BufferRecipeIter->second);
					// The buffer recipe now contains the lowest common denominator of the input recipe and the recipe map recipe

					if (BufferRecipeIter == --BufferRecipe.Ingredients.end())
					{

						//YEP ANOTHER ITERATOR
						for (int32 IntegerBufferIterator = 0; IntegerBufferIterator < IntegerBuffer.Num(); IntegerBufferIterator++)
						{
							// Find the lowest value of the lowest common denominator
							if (SmallestValue > IntegerBuffer[IntegerBufferIterator])
							{
								SmallestValue = IntegerBuffer[IntegerBufferIterator];
							}
						}

						//Multiply BOTH recipes by the denominator

						//Input recipe
						for (auto AlmostDoneRecipeIter = GoodRecipe.first.Ingredients.begin(); AlmostDoneRecipeIter != GoodRecipe.first.Ingredients.end(); AlmostDoneRecipeIter++)
						{
							GoodRecipe.first.Ingredients[AlmostDoneRecipeIter->first] = GoodRecipe.first.Ingredients[AlmostDoneRecipeIter->first] * SmallestValue;
						}

						//Output Recipe
						for (auto AlmostDoneRecipeReturnIter = GoodRecipe.second.Ingredients.begin(); AlmostDoneRecipeReturnIter != GoodRecipe.second.Ingredients.end(); AlmostDoneRecipeReturnIter++)
						{
							//Construct the output recipe by multiplying by the lowest common denominator
							GoodRecipe.second.Ingredients[AlmostDoneRecipeReturnIter->first] = GoodRecipe.second.Ingredients[AlmostDoneRecipeReturnIter->first] * SmallestValue;


							//Add the outputs to the input recipe or the final recipe
							if (InRecipe.Ingredients.count(AlmostDoneRecipeReturnIter->first) > 0)
							{
								InRecipe.Ingredients[AlmostDoneRecipeReturnIter->first] = InRecipe.Ingredients[AlmostDoneRecipeReturnIter->first] + AlmostDoneRecipeReturnIter->second;
							}
							else
							{
								InRecipe.Ingredients.insert(std::make_pair(AlmostDoneRecipeReturnIter->first, AlmostDoneRecipeReturnIter->second));
							}
						}

						// Subtract the input recipe, add the output recipe, bingo bango bongo
						for (auto InputRecipeIter = InRecipe.Ingredients.begin(); InputRecipeIter != InRecipe.Ingredients.end(); InputRecipeIter++)
						{
							//Subtract the input reagents from the input recipe
							InRecipe.Ingredients[InputRecipeIter->first] = InRecipe.Ingredients[InputRecipeIter->first] - GoodRecipe.first.Ingredients[InputRecipeIter->first];

							if (InputRecipeIter == --InRecipe.Ingredients.end())
							{
								return InRecipe;
							}
						}
					}
				}
			}
		}
	}
	return InRecipe;
}

