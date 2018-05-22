
#include <math.h>

#include "stk/PID.h"

stk::PID::PID():
isResetIntegrale(true),
kp(0.0),
ki(0.0),
kd(0.0),
error(0.0),
errorCumulee(0.0),
errorDerivee(0.0),
isInit(false)
{
}


stk::PID::PID(double kp, double ki, double kd, bool ResetIntegrale) :
	isResetIntegrale(ResetIntegrale),
	kp(kp),
	ki(ki),
	kd(kd),
	error(0.0),
	errorCumulee(0.0),
	errorDerivee(0.0),
	isInit(true)
{
}

#ifndef _RTX
std::ostream& stk::PID::describeParams(std::ostream& out)
{
	out << "	kp = " << kp << std::endl;
	out << "	ki = " << ki << std::endl;
	out << "	kd = " << kd << std::endl;
	out << "	reset integrale = " << (isResetIntegrale ? "oui" : "non") << std::endl;
	return out;
}
#endif //_RTX

bool stk::PID::isInitialised()
{
	return isInit;
}


double stk::PID::integre(double ValeurActuelle, double ValeurConsigne, double DeltaT)
{
	//maintenant, on met a jour l'erreur
	double NewError = ValeurConsigne - ValeurActuelle;

	// Si on change de signe d'erreur, reset de l'integrateur
	if (isResetIntegrale && error != 0)
		if ((NewError / error) < 0.0)
			errorCumulee = 0.0;

	// on int�gre l'erreur
	errorCumulee += DeltaT * NewError;
	
	//on d�rive l'erreur
	errorDerivee = (DeltaT > 0.0) ? (NewError - error) / DeltaT : 0.0;

	error = NewError;
	
	double increment = kp * error + ki * errorCumulee + kd * errorDerivee;

	// Remove because it create oscillations when using variable step solver.
	//increment *= DeltaT/periodeReglage; //on adapte la reponse au pas de calcul qui peut varier

	return increment; 	
}
