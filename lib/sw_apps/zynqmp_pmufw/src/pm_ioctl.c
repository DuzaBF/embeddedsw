/*
* Copyright (c) 2021 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
 */

#include "xpfw_config.h"

#ifdef ENABLE_IOCTL
#include "pm_ioctl.h"
#include "pm_common.h"

#ifdef ENABLE_RUNTIME_OVERTEMP
#include "xpfw_mod_overtemp.h"
#endif

#ifdef ENABLE_RUNTIME_OVERTEMP
static u32 OverTempState = 0U;
#endif /* ENABLE_RUNTIME_OVERTEMP */

/**
 * PmSetFeatureConfig() - The feature can be configured by using IOCTL.
 * @configId	The config id of the feature to be configured
 * @value	The value to be configured
 *
 * @return	XST_SUCCESS if successful else XST_FAILURE or an error
 * 		code or a reason code
 */
s32 PmSetFeatureConfig(XPm_FeatureConfigId configId, u32 value)
{
	s32 status = XST_FAILURE;

	switch (configId) {
#ifdef ENABLE_RUNTIME_OVERTEMP
	case XPM_FEATURE_OVERTEMP_STATUS:
		if ((1U == value) && (0U == OverTempState)) {
			/* Initialize over temperature */
			status = OverTempCfgInit();
			OverTempState = 1U;
		} else if ((0U == value) && (1U == OverTempState)) {
			/* De-initialize over temperature */
			status = OverTempCfgDeInit();
			OverTempState = 0U;
		} else {
			status = XST_INVALID_PARAM;
		}
		break;
	case XPM_FEATURE_OVERTEMP_VALUE:
		SetOverTempLimit(value);
		status = XST_SUCCESS;
		break;
#endif /* ENABLE_RUNTIME_OVERTEMP */
	default:
		status = XST_INVALID_PARAM;
		break;
	}

	return status;
}

/**
 * PmGetFeatureConfig() - Get the configured value of thee feature.
 * @configId	The config id of the feature to be queried
 * @value	return by reference value
 *
 * @return	XST_SUCCESS if successful else XST_FAILURE or an error
 * 		code or a reason code
 */
s32 PmGetFeatureConfig(XPm_FeatureConfigId configId, u32 *value)
{
	s32 status = XST_FAILURE;

	switch (configId) {
#ifdef ENABLE_RUNTIME_OVERTEMP
	case XPM_FEATURE_OVERTEMP_STATUS:
		*value = OverTempState;
		status = XST_SUCCESS;
		break;
	case XPM_FEATURE_OVERTEMP_VALUE:
		*value = GetOverTempLimit();
		status = XST_SUCCESS;
		break;
#endif /* ENABLE_RUNTIME_OVERTEMP */
	default:
		status = XST_INVALID_PARAM;
		break;
	}

	return status;
}
#endif /* ENABLE_IOCTL */
