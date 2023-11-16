#include <iostream>
#include <filesystem>
#include "DSGen.h"

int main()
{
    // First set any path you want. The files will be at your_path/Dumpspace/...

    const auto path =  std::filesystem::current_path();

    // You have to set a directory
    DSGen::setDirectory(path);

    // Example adding offsets

    // Make sure the offset value is subtracted from the Base address, offsets like
    // 0x7FF... are not really helpful
    // You only have to call DSGen::addOffset, the rest handles the library

    DSGen::addOffset("OFFSET_NAME", 0x123DEAD);
    DSGen::addOffset("OFFSET_NAME2", 44556677);

    // Example creating a empty class or struct

    // If you dont know some data when creating the class or struct such as size
    // you can always directly modify the resulting ClassHolder struct.
    // Make sure if the class is inherited to add the size of the inherited class

    // Example: No inherited class
    auto someClass = DSGen::createStructOrClass("UObject", true, 0x300);

    // Example: Inherited Classes, from first to root
    auto inheritedClass = DSGen::createStructOrClass("AFortWeapon", true, 0x450 + 0x400, 
        {"AActor", "UObject"}); // inheritance: AActor > UObject

    // Creating a struct is just simply setting the isClass boolean to false
    auto someStruct = DSGen::createStructOrClass("FVector", false, 0xC);

    // You can change the size at any time you want
    someClass.classSize = 0x330;


    // Example adding members to a class or struct

    // There are two ways:
    // 1. You can call the easy DSGen::addMemberToStructOrClass to add a member to the class, *however*
    //    you can ONLY use this if the members type is NOT a template type, for example TArray<xxx>.
    //    Any member that has a template type has to be first created with DSGen::crateMemberType (2.)
    //
    // 2. You can call DSGen::crateMemberType which will create a memberType struct which contains the
    //    subtypes param, where you can add all the template subtypes, for example for a TArray<abc> the
    //    tye abc is the template subtype, it has to be added to the subtype param. Once created, you can
    //    call DSGen::addMemberToStructOrClass with the MemberType to add it to the class.

    // Do not add unknown members that are only for padding!
    // The EType is very important!! Before adding a member, make sure to know what type the member itself is.
    // For more information what a EType is, loot at DSGen.h Ln:11
    

    // Example for 1.
    // Make sure to add it to the correct class. The EType is very important!! Before adding a member,
    // make sure to know what type the member itself is.
    DSGen::addMemberToStructOrClass(
        someClass, // the class where the member should get added
        "Location",  // the name of the member
        DSGen::ET_Struct, // the type of the member
        "FVector", // the type name of the member
        "", // if the member is a pointer, e.g ABC* xyz, the extendedType must be "*" but the type name only ABC (!)
        0x30, 0xC); // the offset and size of the member. If bit, set the bitOffset from 0 - 7;

    // Example for 1.
    // This time we add the member ULevel* persistentLevel. The pointer is not a part of the type name,
    // so make sure to set the type name to ULevel and the extendedType to *.
    DSGen::addMemberToStructOrClass(
        someClass,
        "PersistentLevel",
        DSGen::ET_Class, // this time its a Class
        "ULevel", // we set the type name to ULvel
        "*", // we set the extendedType to *
        0x3C, 0x8);

    // Example for 1.
    // This time we add a bitfield. All we have to do is set the bitOffset. The Generator expects that every added
    // bit is exactly 1 bit large, not more.
    DSGen::addMemberToStructOrClass(
        someClass,
        "bLoading",
        DSGen::ET_Default, // this is a default member. Default members are chars, ints, undefined types etc.
        "unsigned char",
        "", // no extended type
        0x3C, 0x1, 3); // size is 1, howeber bitOffet is 3


    // Example for 2.

    // 2. is only needed if the member you're trying to add is a template type.
    // This includes TArray<abc>, TMap<abc, def>, Something<pair<abc, def>, ghi>...
    // When creating a template member, you first have to create the template subtype member
    // and then the template member itself.
    // Ignore the isReference type, this is only important for function params

    // For example, we use the member TMap<int, UClass*> myMap
    // we first create the int
    auto subTypeInt = DSGen::createMemberType(
        DSGen::ET_Default, // default, as an int is no struct, class, or whatever
        "int");

    auto subTypeUClass = DSGen::createMemberType(
        DSGen::ET_Class, // UClass* is a class
        "UClass", // remove the * (see 1. for more info)
        "*" // add the * here
    );

    // Now we create the TMap

    auto tArrayType = DSGen::createMemberType(
        DSGen::ET_Class,
        "TMap",
        "", // no extended type
        { subTypeInt, subTypeUClass }); // add the subtypes here, first int then uClass

    // now we add the TMap to the class

    // Now we only have to define the name, memberType, offset and size
    DSGen::addMemberToStructOrClass(someClass, "myMap", tArrayType, 0x300, 0x50);

    // Do not bake the class or struct now if you plan on adding functions to the class

    // Example creating a function

	// Imagine following function:
    // void OnPlayImpactFX(
    //      FHitResult& HitResult,
    //      TEnumAsByte<EPhysicalSurface> ImpactPhysicalSurface,
    //      UFXSystemComponent* SpawnedPSC
    //);


    // The returntype works just like when creating a memberType


    // if your function has no return type, just create a void type
    auto functionReturnType = DSGen::createMemberType(
        DSGen::ET_Default, // default, as an void is no struct, class, or whatever
        "void", "");

    // Now to the members. If the member is a reference (&), the isReference bool has to be set.
    auto functionParam1 = DSGen::createMemberType(
        DSGen::ET_Struct,
        "FHitResult", 
        "", {}, //leavve extendedType and subtypes empty
        true); //set the isReference bool to true

    auto functionSubParam2 = DSGen::createMemberType(
        DSGen::ET_Enum,
        "EPhysicalSurface");

    auto functionParam2 = DSGen::createMemberType(
        DSGen::ET_Class, 
        "TEnumAsByte",
        "", { functionSubParam2 }); // add the subtype

    auto functionParam3 = DSGen::createMemberType(
        DSGen::ET_Class,
        "UFXSystemComponent",
        "*");

    //now create the vector pair

    std::vector<std::pair<DSGen::MemberType, std::string>> paramTypeAndNames;
    paramTypeAndNames.push_back(std::pair(functionParam1, "HitResult"));
    paramTypeAndNames.push_back(std::pair(functionParam2, "ImpactPhysicalSurface"));
    paramTypeAndNames.push_back(std::pair(functionParam3, "SpawnedPSC"));

    // Now create the function

    // The owningclass just has to be its name
    DSGen::createFunction(
        someClass, // owning class where the function resides in
        "OnPlayImpactFX", //function name
        "RequiredAPI|Native|Event|Protected|HasOutParms|BlueprintEvent", //function flags, if known
        0x1907e4, // function offset, if known
        functionReturnType, // function return type
        paramTypeAndNames // function params
    );

    // FINAL STEP FOR class creation
	// DONT FORGET TO BAKE THE STRUCT OR CLASS!
	// after baking, dont change anything on the class anymore.
    DSGen::bakeStructOrClass(someClass);

    // Example creating a enum

    // Probably the simplest out of all
    // for example this enum:
    //enum ACLCompressionLevel : uint8_t {
    //    ACLCL_Lowest = 0,
    //    ACLCL_Low = 1,
    //    ACLCL_Medium = 2,
    //    ACLCL_High = 3,
    //    ACLCL_Highest = 4,
    //    ACLCL_MAX = 5
    //
    //};

	// first all the members have to get added to a vector
    std::vector<std::pair<std::string, int>> members;

    members.push_back(std::pair("ACLCL_Lowest", 0));
    members.push_back(std::pair("ACLCL_Low", 1));
    members.push_back(std::pair("ACLCL_Medium", 2));
    members.push_back(std::pair("ACLCL_High", 3));
    members.push_back(std::pair("ACLCL_Highest", 4));
    members.push_back(std::pair("ACLCL_MAX", 5));

    // now create the enum
    auto enu = DSGen::createEnum("ACLCompressionLevel", "uint8_t", members);

    // finally, we bake the enum
    DSGen::bakeEnum(enu);

    // Now if done, just call Dump
    DSGen::dump();
}