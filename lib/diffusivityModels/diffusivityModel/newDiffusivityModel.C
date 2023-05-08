/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "error.H" // OpenFOAM src file
#include "diffusivityModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Code changed and converted to openfoam-4.0 (J.P.Kone, 07.07.2016)

namespace Foam
{
namespace diffusivityModels
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

autoPtr<diffusivityModel> diffusivityModel::New
(
    fvMesh& mesh,
    scalarField& diff,
    const labelList& cells, // List<int>& changed to labelList& (J.P.Kone, 07.07.2016)
    const dictionary& dict
)
{
    word diffTypeName = dict.lookup("type");

    Info<< "Selecting diffusivity model " << diffTypeName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(diffTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "diffusivityModel::New(fvMesh& mesh, scalarField&, "
            "const labelList&)"
        )   << "Unknown diffusivityModel type " << diffTypeName
            << endl << endl
            << "Valid diffusivityModel types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<diffusivityModel>(cstrIter()(mesh, diff, cells, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusivityModels
} // End namespace Foam

// ************************************************************************* //
