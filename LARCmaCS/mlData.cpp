#include "mlData.h"
#include <memory.h>
#include "constants.h"

MlData::MlData(RCConfig conf)
{
	config = conf;

	Ball         = mxCreateDoubleMatrix(1, 3, mxREAL);
	Blue         = mxCreateDoubleMatrix(Constants::maxRobotsInTeam, 4, mxREAL);
	Yellow       = mxCreateDoubleMatrix(Constants::maxRobotsInTeam, 4, mxREAL);
	Rule         = mxCreateDoubleMatrix(Constants::ruleLength, Constants::ruleAmount, mxREAL);
	Back_Params  = mxCreateDoubleMatrix(config.BACK_LENGTH, config.BACK_AMOUNT, mxREAL);
	Back_Amount  = mxCreateDoubleMatrix(1, 1, mxREAL);
	Back_Length  = mxCreateDoubleMatrix(1, 1, mxREAL);
	ballInside   = mxCreateDoubleMatrix(Constants::maxRobotsInTeam, 1, mxREAL);
	state        = mxCreateDoubleScalar(0);
	team		 = mxCreateDoubleScalar(0);
	partOfFieldLeft = mxCreateDoubleScalar(0);
}

MlData::MlData(const MlData &dat)
{
	memset(&config, 0, sizeof(RCConfig));
	memcpy(&config, &dat.config, sizeof(RCConfig));

	Ball         = mxCreateDoubleMatrix(1, 3, mxREAL);
	memcpy(Ball, &dat.Ball, sizeof(Ball));

	Blue         = mxCreateDoubleMatrix(Constants::maxRobotsInTeam, 4, mxREAL);
	memcpy(Blue, &dat.Blue, sizeof(Blue));

	Yellow       = mxCreateDoubleMatrix(Constants::maxRobotsInTeam, 4, mxREAL);
	memcpy(Yellow, &dat.Yellow, sizeof(Yellow));

	Rule         = mxCreateDoubleMatrix(config.RULE_LENGTH, config.RULE_AMOUNT, mxREAL);
	memcpy(Rule, &dat.Rule, sizeof(Rule));

	Back_Params  = mxCreateDoubleMatrix(config.BACK_LENGTH, config.BACK_AMOUNT, mxREAL);
	memcpy(Back_Params, &dat.Back_Params, sizeof(Back_Params));

	Back_Amount  = mxCreateDoubleMatrix(1, 1, mxREAL);
	memcpy(Back_Amount, &dat.Back_Amount, sizeof(Back_Amount));

	Back_Length  = mxCreateDoubleMatrix(1, 1, mxREAL);
	memcpy(Back_Length, &dat.Back_Length, sizeof(Back_Length));

	ballInside   = mxCreateDoubleMatrix(Constants::maxRobotsInTeam, 1, mxREAL);
	memcpy(ballInside, &dat.ballInside, sizeof(ballInside));

	state      = mxCreateDoubleScalar(0);
	memcpy(state, &dat.state, sizeof(state));

	team       = mxCreateDoubleScalar(0);
	memcpy(team, &dat.team, sizeof(team));

	partOfFieldLeft = mxCreateDoubleScalar(0);
	memcpy(partOfFieldLeft, &dat.partOfFieldLeft, sizeof(partOfFieldLeft));
}
