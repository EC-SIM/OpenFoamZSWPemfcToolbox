################################################################################
# User can select between serial (runSerial) or parallel (runParallel) simulation
All: clean mesh simulationSetup runParallel 
mesh: bmesh regionMesh defineZones
###############
clean:
	(rm -rf *.log)
	(rm -rf dynamicCode)
	(rm -rf ./system/*/)
	(rm -rf ./constant/*)
	(rm -rf ./0/*)
	(rm -rf ./processor*)
	(foamListTimes -rm)
	(rm -rf ./system/blockMeshDict)
	(rm -rf ./system/decomposeParDict)

	(./Allclean)
###############
bmesh:
	(cp -rf ./config/system/blockMeshDict ./system/blockMeshDict)
	(cp -rf ./config/system/controlDict.blockMesh ./system/controlDict)
	(blockMesh >blockMesh.log)
	(grep -l 'empty' ./constant/polyMesh/boundary | xargs sed -i 's/empty/wall/g')
	(grep -l 'defaultFaces' ./constant/polyMesh/boundary | xargs sed -i 's/defaultFaces/wall/g')

regionMesh:
	(splitMeshRegions -cellZonesOnly -overwrite >splitMeshRegions.log)
	(rm -rf ./0/*)

defineZones:
	(cp -rf ./config/system/air/topoSetDict.cellSetsAir ./system/air/topoSetDict)
	(topoSet -region air >topoSetDict.cellSetsAir.log)
	(cp -rf ./config/system/fuel/topoSetDict.cellSetsFuel ./system/fuel/topoSetDict)
	(topoSet -region fuel >topoSetDict.cellSetsFuel.log)

simulationSetup:
	(rm -rf ./0)
	(cp -rf ./config/0 .)
	(cp -rf ./config/constant/rxnProperties ./constant)
	(cp -rf ./config/constant/cellProperties ./constant)
	(cp -rf ./config/constant/air/* ./constant/air)
	(cp -rf ./config/constant/fuel/* ./constant/fuel)
	(cp -rf ./config/constant/coolant/* ./constant/coolant)
	(cp -rf ./config/constant/electrolyte/* ./constant/electrolyte)
	(cp -rf ./config/constant/abp/* ./constant/abp)
	(cp -rf ./config/constant/cbp/* ./constant/cbp)
	(cp -rf ./config/system/fv* ./system)
	(cp -rf ./config/system/air/fv* ./system/air)
	(cp -rf ./config/system/fuel/fv* ./system/fuel)
	(cp -rf ./config/system/coolant/fv* ./system/coolant)
	
runParallel:
	(cp -rf ./config/system/controlDict.run ./system/controlDict)

	(cp -rf ./config/system/decomposeParDict ./system/decomposeParDict)
	(./prepareParallelRun)
	(./parallelRun)

	(reconstructPar -latestTime)
	(reconstructPar -region air -latestTime)
	(reconstructPar -region fuel -latestTime)
	(reconstructPar -region coolant -latestTime)
	(reconstructPar -region abp -latestTime)
	(reconstructPar -region cbp -latestTime)
	(reconstructPar -region electrolyte -latestTime)

runSerial:
	(cp -rf ./config/system/controlDict.run ./system/controlDict)
	(./serialRun)


################################################################################
