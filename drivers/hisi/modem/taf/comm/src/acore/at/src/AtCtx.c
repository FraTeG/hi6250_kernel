

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "AtCtx.h"
#include "AtDataProc.h"
#include "AtMntn.h"


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID                 PS_FILE_ID_AT_CTX_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
/***************************AT的全局变量表 Begin******************************/
/* AT与编译两次的PID对应表  */
AT_MODEM_PID_TAB_STRU                   g_astAtModemPidTab[] =
{
    {I0_WUEPS_PID_USIM,         I1_WUEPS_PID_USIM,          I2_WUEPS_PID_USIM,      0},
    {I0_MAPS_STK_PID,           I1_MAPS_STK_PID,            I2_MAPS_STK_PID,        0},
    {I0_MAPS_PIH_PID,           I1_MAPS_PIH_PID,            I2_MAPS_PIH_PID,        0},
    {I0_MAPS_PB_PID,            I1_MAPS_PB_PID,             I2_MAPS_PB_PID,         0},
    {I0_UEPS_PID_GRM,           I1_UEPS_PID_GRM,            I2_UEPS_PID_GRM,        0},
    {I0_UEPS_PID_DL,            I1_UEPS_PID_DL,             I2_UEPS_PID_DL,         0},
    {I0_UEPS_PID_LL,            I1_UEPS_PID_LL,             I2_UEPS_PID_LL,         0},
    {I0_UEPS_PID_SN,            I1_UEPS_PID_SN,             I2_UEPS_PID_SN,         0},
    {I0_UEPS_PID_GAS,           I1_UEPS_PID_GAS,            I2_UEPS_PID_GAS,        0},
    {I0_WUEPS_PID_MM,           I1_WUEPS_PID_MM,            I2_WUEPS_PID_MM,        0},
    {I0_WUEPS_PID_MMC,          I1_WUEPS_PID_MMC,           I2_WUEPS_PID_MMC,       0},
    {I0_WUEPS_PID_GMM,          I1_WUEPS_PID_GMM,           I2_WUEPS_PID_GMM,       0},
    {I0_WUEPS_PID_MMA,          I1_WUEPS_PID_MMA,           I2_WUEPS_PID_MMA,       0},
    {I0_WUEPS_PID_CC,           I1_WUEPS_PID_CC,            I2_WUEPS_PID_CC,        0},
    {I0_WUEPS_PID_SS,           I1_WUEPS_PID_SS,            I2_WUEPS_PID_SS,        0},
    {I0_WUEPS_PID_TC,           I1_WUEPS_PID_TC,            I2_WUEPS_PID_TC,        0},
    {I0_WUEPS_PID_SMS,          I1_WUEPS_PID_SMS,           I2_WUEPS_PID_SMS,       0},
    {I0_WUEPS_PID_RABM,         I1_WUEPS_PID_RABM,          I2_WUEPS_PID_RABM,      0},
    {I0_WUEPS_PID_SM,           I1_WUEPS_PID_SM,            I2_WUEPS_PID_SM,        0},
    {I0_WUEPS_PID_ADMIN,        I1_WUEPS_PID_ADMIN,         I2_WUEPS_PID_ADMIN,     0},
    {I0_WUEPS_PID_TAF,          I1_WUEPS_PID_TAF,           I2_WUEPS_PID_TAF,       0},
    {I0_WUEPS_PID_VC,           I1_WUEPS_PID_VC,            I2_WUEPS_PID_VC,        0},
    {I0_WUEPS_PID_DRV_AGENT,    I1_WUEPS_PID_DRV_AGENT,     I2_WUEPS_PID_DRV_AGENT, 0},
    {I0_UEPS_PID_MTA,           I1_UEPS_PID_MTA,            I2_UEPS_PID_MTA,        0},
    {I0_DSP_PID_GPHY,           I1_DSP_PID_GPHY,            I2_DSP_PID_GPHY,        0},
    {I0_DSP_PID_SLEEP,          I1_DSP_PID_SLEEP,           I2_DSP_PID_SLEEP,       0},
    {I0_DSP_PID_APM,            I1_DSP_PID_APM,             I2_DSP_PID_APM,         0},
    {I0_WUEPS_PID_SLEEP,        I1_WUEPS_PID_SLEEP,         I2_WUEPS_PID_SLEEP,     0},
    {I0_WUEPS_PID_WRR,          I1_WUEPS_PID_WRR,           0,                      0},
    {I0_WUEPS_PID_WCOM,         I1_WUEPS_PID_WCOM,          0,                      0},
    {I0_DSP_PID_WPHY,           I1_DSP_PID_WPHY,            0,                      0},
};

VOS_UINT32                              g_ulCtzuFlag = 0;

/* AT模块公共上下文 */
AT_COMM_CTX_STRU                        g_stAtCommCtx;

/* AT模块与Modem相关的上下文 */
AT_MODEM_CTX_STRU                       g_astAtModemCtx[MODEM_ID_BUTT];

/* AT模块与Client相关的上下文 */
AT_CLIENT_CTX_STRU                      g_astAtClientCtx[AT_MAX_CLIENT_NUM];

/* AT模块复位相关的上下文 */
AT_RESET_CTX_STRU                       g_stAtResetCtx;

/***************************AT的全局变量表 End******************************/

/*********************************后续需要调整的*************************************/
/* 是否具有权限标志(控制DIAG/SHELL口的权限) */
AT_E5_RIGHT_FLAG_ENUM_U32               g_enATE5RightFlag;

/* 由于 g_stATDislogPwd 中的 DIAG口的状态要放入备份NV列表; 而密码不用备份
   故将 g_stATDislogPwd 中的密码废弃, 仅使用其中的 DIAG 口状态;
   重新定义NV项用来保存密码  */
VOS_INT8                                g_acATOpwordPwd[AT_OPWORD_PWD_LEN+1];

/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, begin */
/* 用于当前是否有权限操作AT端口 */
AT_RIGHT_OPEN_FLAG_STRU                 g_stAtRightOpenFlg;
/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, begin */

/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, begin */
/* 使能禁止SD卡时需要的密码 */
VOS_INT8                                g_acATE5DissdPwd[AT_DISSD_PWD_LEN+1];
/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, end */

/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, begin */
/*命令不支持提示字串*/
VOS_UINT8                               gaucAtCmdNotSupportStr[AT_NOTSUPPORT_STR_LEN+4];
/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, end */

AT_ABORT_CMD_CTX_STRU                   gstAtAbortCmdCtx;   /* 用于保存打断的信息 */

/*纪录拨号错误码是否使能 */
PPP_DIAL_ERR_CODE_ENUM                  gucPppDialErrCodeRpt;

AT_DIAL_CONNECT_DISPLAY_RATE_STRU       g_stDialConnectDisplayRate;

/* UE下行能力: 包含协议版本和能力值 */
AT_DOWNLINK_RATE_CATEGORY_STRU          g_stAtDlRateCategory;

VOS_UINT8                               ucAtS3          = 13;                   /* <CR> */
VOS_UINT8                               ucAtS4          = 10;                   /* <LF> */
VOS_UINT8                               ucAtS5          = 8;                    /* <DEL> */
VOS_UINT8                               ucAtS6          = 2;                    /* Number of seconds to wait before blind dialling:default value = 2 */
VOS_UINT8                               ucAtS7          = 50;                   /* Number of seconds in which connection must be established or call will be disconnected,
                                                                                   default value = 50(refer to Q)*/

/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, begin */
AT_CMEE_TYPE                            gucAtCmeeType;                          /* E5错误码默认错误编号 */
/* Modified by s62952 for BalongV300R002 Build优化项目 2012-02-28, end */

TAF_UINT32                              g_ulSTKFunctionFlag = TAF_FALSE;

/*********************************CC Begin*************************************/
/* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-10, begin */
/* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-10, end */
/*********************************CC End*************************************/

/*********************************SMS Begin*************************************/
MN_MSG_CLASS0_TAILOR_U8                 g_enClass0Tailor         = MN_MSG_CLASS0_DEF;
/*********************************SMS End*************************************/

/*********************************NET Begin*************************************/
VOS_UINT16                              g_usReportCregActParaFlg = VOS_FALSE;
CREG_CGREG_CI_RPT_BYTE_ENUM             gucCiRptByte = CREG_CGREG_CI_RPT_TWO_BYTE;

/*********************************NET End*************************************/
NVIM_RSRP_CFG_STRU                      g_stRsrpCfg;
NVIM_RSCP_CFG_STRU                      g_stRscpCfg;
NVIM_ECIO_CFG_STRU                      g_stEcioCfg;

AT_ACCESS_STRATUM_RELEASE_STRU          g_stReleaseInfo;

AT_SS_CUSTOMIZE_PARA_STRU               g_stAtSsCustomizePara;

AT_TRACE_MSGID_TAB_STRU                 g_stAtTraceMsgIdTab[AT_CLIENT_ID_BUTT];

AT_CLIENT_CFG_MAP_TAB_STRU              g_astAtClientCfgMapTbl[] =
{
    AT_CLIENT_CFG_ELEMENT(PCUI),
    AT_CLIENT_CFG_ELEMENT(CTRL),
    AT_CLIENT_CFG_ELEMENT(PCUI2),
    AT_CLIENT_CFG_ELEMENT(MODEM),
    AT_CLIENT_CFG_ELEMENT(NDIS),
    AT_CLIENT_CFG_ELEMENT(UART),
    AT_CLIENT_CFG_ELEMENT(SOCK),
    AT_CLIENT_CFG_ELEMENT(APPSOCK),
    AT_CLIENT_CFG_ELEMENT(APP),
    AT_CLIENT_CFG_ELEMENT(APP1),
    AT_CLIENT_CFG_ELEMENT(APP2),
    AT_CLIENT_CFG_ELEMENT(APP3),
    AT_CLIENT_CFG_ELEMENT(APP4),
    AT_CLIENT_CFG_ELEMENT(APP5),
    AT_CLIENT_CFG_ELEMENT(APP6),
    AT_CLIENT_CFG_ELEMENT(APP7),
    AT_CLIENT_CFG_ELEMENT(APP8),
    AT_CLIENT_CFG_ELEMENT(APP9),
    AT_CLIENT_CFG_ELEMENT(APP10),
    AT_CLIENT_CFG_ELEMENT(APP11),
    AT_CLIENT_CFG_ELEMENT(APP12),
    AT_CLIENT_CFG_ELEMENT(APP13),
    AT_CLIENT_CFG_ELEMENT(APP14),
    AT_CLIENT_CFG_ELEMENT(APP15),
    AT_CLIENT_CFG_ELEMENT(APP16),
    AT_CLIENT_CFG_ELEMENT(APP17),
    AT_CLIENT_CFG_ELEMENT(APP18),
    AT_CLIENT_CFG_ELEMENT(APP19),
    AT_CLIENT_CFG_ELEMENT(APP20),
    AT_CLIENT_CFG_ELEMENT(APP21),
    AT_CLIENT_CFG_ELEMENT(APP22),
    AT_CLIENT_CFG_ELEMENT(APP23),
    AT_CLIENT_CFG_ELEMENT(APP24),
    AT_CLIENT_CFG_ELEMENT(APP25),
    AT_CLIENT_CFG_ELEMENT(APP26),
};

const VOS_UINT8                         g_ucAtClientCfgMapTabLen = AT_ARRAY_SIZE(g_astAtClientCfgMapTbl);




/*****************************************************************************
  3 函数实现
*****************************************************************************/
/*****************************************************************************
 函 数 名  : AT_InitUsimStatus
 功能描述  : 初始化USIM状态
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月26日
    作    者   : l60609
    修改内容   : 新生成函数
  2.日    期   : 2013年4月3日
    作    者   : s00217060
    修改内容   : 主动上报AT命令控制下移至C核

*****************************************************************************/
VOS_VOID AT_InitUsimStatus(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_USIM_INFO_CTX_STRU              *pstUsimInfoCtx = VOS_NULL_PTR;

    pstUsimInfoCtx = AT_GetUsimInfoCtxFromModemId(enModemId);

    pstUsimInfoCtx->enCardStatus = USIMM_CARD_SERVIC_BUTT;
    pstUsimInfoCtx->enCardType   = USIMM_CARD_TYPE_BUTT;

    pstUsimInfoCtx->ucIMSILen    = 0;
    TAF_MEM_SET_S(pstUsimInfoCtx->aucIMSI, sizeof(pstUsimInfoCtx->aucIMSI), 0x00, sizeof(pstUsimInfoCtx->aucIMSI));

    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, begin */
    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitPlatformRatList
 功能描述  : 初始化平台能力
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月26日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_InitPlatformRatList(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_SPT_RAT_STRU              *pstSptRat   = VOS_NULL_PTR;
    VOS_UINT8                          *pucIsCLMode = VOS_NULL_PTR;

    pstSptRat = AT_GetSptRatFromModemId(enModemId);

    /* 默认情况下单板只支持GSM */
    pstSptRat->ucPlatformSptGsm        = VOS_TRUE;
    pstSptRat->ucPlatformSptWcdma      = VOS_FALSE;
    pstSptRat->ucPlatformSptLte        = VOS_FALSE;
    pstSptRat->ucPlatformSptUtralTDD   = VOS_FALSE;

    pucIsCLMode  = AT_GetModemCLModeCtxAddrFromModemId(enModemId);
    *pucIsCLMode = VOS_FALSE;

    return;

}

/*****************************************************************************
 函 数 名  : AT_InitCommPsCtx
 功能描述  : 初始化PS域公共上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : l60609
    修改内容   : 新生成函数

  2.日    期   : 2013年6月7日
    作    者   : z00214637
    修改内容   : V3R3 Share-PDP项目修改

  3.日    期   : 2015年3月30日
    作    者   : A00165503
    修改内容   : SPE Project: SPE适配
*****************************************************************************/
VOS_VOID AT_InitCommPsCtx(VOS_VOID)
{
    AT_COMM_PS_CTX_STRU                *pstPsCtx = VOS_NULL_PTR;

    TAF_MEM_SET_S(&g_stAtNdisDhcpPara, sizeof(g_stAtNdisDhcpPara), 0x00, sizeof(g_stAtNdisDhcpPara));
    TAF_MEM_SET_S(&g_stAtAppPdpEntity, sizeof(g_stAtAppPdpEntity), 0x00, sizeof(g_stAtAppPdpEntity));

    pstPsCtx = AT_GetCommPsCtxAddr();

    pstPsCtx->ucIpv6Capability = AT_IPV6_CAPABILITY_IPV4_ONLY;

    pstPsCtx->stIpv6BackProcExtCauseTbl.ulCauseNum = 0;

    pstPsCtx->ulIpv6AddrTestModeCfg = 0;

    pstPsCtx->ucSharePdpFlag = VOS_FALSE;

    pstPsCtx->lSpePort       = AT_INVALID_SPE_PORT;
    pstPsCtx->ulIpfPortFlg   = VOS_FALSE;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitCommPbCtx
 功能描述  : 初始化电话本公共上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月12日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitCommPbCtx(VOS_VOID)
{
    AT_COMM_PB_CTX_STRU                *pstCommPbCntxt = VOS_NULL_PTR;

    pstCommPbCntxt = AT_GetCommPbCtxAddr();

    pstCommPbCntxt->usCurrIdx       = 0;
    pstCommPbCntxt->usLastIdx       = 0;
    pstCommPbCntxt->ulSingleReadFlg = VOS_FALSE;
    return;
}

/*****************************************************************************
 函 数 名  : AT_GetCmdProcCtxAddr
 功能描述  : 初始化上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_CMD_PROC_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月10日
    作    者   : z00301431
    修改内容   : 新生成函数
*****************************************************************************/
AT_CMD_PROC_CTX_STRU* AT_GetCmdProcCtxAddr(VOS_VOID)
{
    return &(g_stAtCommCtx.stCmdProcCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetAuthPubkeyExCmdCtxAddr
 功能描述  : 初始化上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_AUTH_PUBKEYEX_CMD_PROC_CTX*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月10日
    作    者   : z00301431
    修改内容   : 新生成函数
*****************************************************************************/
AT_AUTH_PUBKEYEX_CMD_PROC_CTX* AT_GetAuthPubkeyExCmdCtxAddr(VOS_VOID)
{
    return &(AT_GetCmdProcCtxAddr()->stAuthPubkeyExCmdCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetCmdVmsetCtxAddr
 功能描述  : 获取vmset地址
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_VMSET_CMD_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年6月16日
    作    者   : h00360002
    修改内容   : 新生成函数
*****************************************************************************/
AT_VMSET_CMD_CTX_STRU* AT_GetCmdVmsetCtxAddr(VOS_VOID)
{
    return &(AT_GetCmdProcCtxAddr()->stVmSetCmdCtx);
}

/*****************************************************************************
 函 数 名  : AT_InitVmSetCtx
 功能描述  : 初始化VmSetCtx上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年6月18日
    作    者   : h00360002
    修改内容   : 新增
*****************************************************************************/
VOS_VOID AT_InitVmSetCtx(VOS_VOID)
{
    AT_VMSET_CMD_CTX_STRU              *pstVmSetCmdCtx        = VOS_NULL_PTR;

    pstVmSetCmdCtx = AT_GetCmdVmsetCtxAddr();

    pstVmSetCmdCtx->ulReportedModemNum  = 0;
    pstVmSetCmdCtx->ulResult            = AT_OK;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitCmdProcCtx
 功能描述  : 初始化上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月10日
    作    者   : z00301431
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitCmdProcCtx(VOS_VOID)
{
    AT_AUTH_PUBKEYEX_CMD_PROC_CTX      *pstAuthPubkeyExCmdCtx = VOS_NULL_PTR;

    pstAuthPubkeyExCmdCtx = AT_GetAuthPubkeyExCmdCtxAddr();

    pstAuthPubkeyExCmdCtx->ucClientId               = 0;
    pstAuthPubkeyExCmdCtx->ucCurIdx                 = 0;
    pstAuthPubkeyExCmdCtx->ucTotalNum               = 0;
    pstAuthPubkeyExCmdCtx->usParaLen                = 0;
    pstAuthPubkeyExCmdCtx->ucSettingFlag            = VOS_FALSE;
    pstAuthPubkeyExCmdCtx->pucData                  = VOS_NULL_PTR;
    pstAuthPubkeyExCmdCtx->hAuthPubkeyProtectTimer  = VOS_NULL_PTR;

    AT_InitVmSetCtx();

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitMsgNumCtrlCtx
 功能描述  : 初始化at消息队列数量控制上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年09月27日
    作    者   : m00217266
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitMsgNumCtrlCtx(VOS_VOID)
{
    AT_CMD_MSG_NUM_CTRL_STRU           *pstMsgNumCtrlCtx = VOS_NULL_PTR;

    pstMsgNumCtrlCtx = AT_GetMsgNumCtrlCtxAddr();

    /* 锁初始化 */
    VOS_SpinLockInit(&(pstMsgNumCtrlCtx->stSpinLock));

    pstMsgNumCtrlCtx->ulMsgCount = 0;

    return;
}

/*****************************************************************************
 函 数 名  : AT_ClearAuthPubkeyCtx
 功能描述  : 初始化上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月10日
    作    者   : z00301431
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_ClearAuthPubkeyCtx(VOS_VOID)
{
    AT_AUTH_PUBKEYEX_CMD_PROC_CTX      *pstAuthPubkeyExCmdCtx = VOS_NULL_PTR;

    pstAuthPubkeyExCmdCtx = AT_GetAuthPubkeyExCmdCtxAddr();

    pstAuthPubkeyExCmdCtx->ucClientId               = 0;
    pstAuthPubkeyExCmdCtx->ucCurIdx                 = 0;
    pstAuthPubkeyExCmdCtx->ucTotalNum               = 0;
    pstAuthPubkeyExCmdCtx->usParaLen                = 0;
    pstAuthPubkeyExCmdCtx->ucSettingFlag            = VOS_FALSE;

    if (VOS_NULL_PTR != pstAuthPubkeyExCmdCtx->pucData)
    {
        PS_MEM_FREE(WUEPS_PID_AT, pstAuthPubkeyExCmdCtx->pucData);
        pstAuthPubkeyExCmdCtx->pucData = VOS_NULL_PTR;
    }

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemCcCtx
 功能描述  : 初始化呼叫相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数
  2.日    期   : 2013年04月11日
    作    者   : l00198894
    修改内容   : C50问题单同步
  3.日    期   : 2013年07月11日
    作    者   : l00198894
    修改内容   : V9R1 STK升级项目
*****************************************************************************/
VOS_VOID AT_InitModemCcCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_CC_CTX_STRU               *pstCcCtx = VOS_NULL_PTR;
    VOS_UINT8                           i;

    pstCcCtx = AT_GetModemCcCtxAddrFromModemId(enModemId);

    pstCcCtx->ulCurIsExistCallFlag = VOS_FALSE;
    pstCcCtx->enCsErrCause         = TAF_CS_CAUSE_SUCCESS;

    /* 语音自动应答参数初始化 */
    TAF_MEM_SET_S(&(pstCcCtx->stS0TimeInfo), sizeof(pstCcCtx->stS0TimeInfo), 0x00, sizeof(pstCcCtx->stS0TimeInfo));

    TAF_MEM_SET_S(&(pstCcCtx->stEconfInfo), sizeof(pstCcCtx->stEconfInfo), 0x00, sizeof(pstCcCtx->stEconfInfo));

    for (i = 0; i < TAF_CALL_MAX_ECONF_CALLED_NUM; i++)
    {
        pstCcCtx->stEconfInfo.astCallInfo[i].enCallState = TAF_CALL_ECONF_STATE_BUTT;
        pstCcCtx->stEconfInfo.astCallInfo[i].enCause     = TAF_CS_CAUSE_SUCCESS;
    }

    /* Deleted by l00198894 for V9R1 STK升级, 2013/07/11 */

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemSsCtx
 功能描述  : 初始化SS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数
  2.日    期   : 2013年4月3日
    作    者   : s00217060
    修改内容   : 主动上报AT命令控制下移至C核

*****************************************************************************/
VOS_VOID AT_InitModemSsCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_SS_CTX_STRU               *pstSsCtx = VOS_NULL_PTR;

    pstSsCtx = AT_GetModemSsCtxAddrFromModemId(enModemId);

    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */
    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */
    pstSsCtx->usUssdTransMode      = AT_USSD_TRAN_MODE;
    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */
    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */
    pstSsCtx->enCModType           = MN_CALL_MODE_SINGLE;
    pstSsCtx->ucSalsType           = AT_SALS_DISABLE_TYPE;
    pstSsCtx->ucClipType           = AT_CLIP_DISABLE_TYPE;
    pstSsCtx->ucClirType           = AT_CLIR_AS_SUBSCRIPT;
    pstSsCtx->ucColpType           = AT_COLP_DISABLE_TYPE;
    pstSsCtx->ucCrcType            = AT_CRC_DISABLE_TYPE;
    pstSsCtx->ucCcwaType           = AT_CCWA_DISABLE_TYPE;

    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */
    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-3, end */

    pstSsCtx->stCbstDataCfg.enSpeed    = MN_CALL_CSD_SPD_64K_MULTI;
    pstSsCtx->stCbstDataCfg.enName     = MN_CALL_CSD_NAME_SYNC_UDI;
    pstSsCtx->stCbstDataCfg.enConnElem = MN_CALL_CSD_CE_T;

    TAF_MEM_SET_S(&(pstSsCtx->stCcugCfg), sizeof(pstSsCtx->stCcugCfg), 0x00, sizeof(pstSsCtx->stCcugCfg));

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemSmsCtx
 功能描述  : 初始化短消息相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数
  2.日    期   : 2014年03月04日
    作    者   : f62575
    修改内容   : DTS2014030801193, +CSMP初始值不生效

*****************************************************************************/
VOS_VOID AT_InitModemSmsCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_SMS_CTX_STRU              *pstSmsCtx = VOS_NULL_PTR;

    pstSmsCtx = AT_GetModemSmsCtxAddrFromModemId(enModemId);

    pstSmsCtx->enCmgfMsgFormat      = AT_CMGF_MSG_FORMAT_PDU;
    pstSmsCtx->ucCsdhType           = AT_CSDH_NOT_SHOW_TYPE;
    pstSmsCtx->ucParaCmsr           = VOS_FALSE;
    pstSmsCtx->ucSmsAutoReply       = VOS_FALSE;
    pstSmsCtx->enCsmsMsgVersion     = MN_MSG_CSMS_MSG_VERSION_PHASE2_PLUS;

    /* ME存储状态初始化 */
    pstSmsCtx->enMsgMeStorageStatus = MN_MSG_ME_STORAGE_DISABLE;
    pstSmsCtx->ucLocalStoreFlg      = VOS_TRUE;

    pstSmsCtx->stCnmiType.CnmiModeType    = AT_CNMI_MODE_BUFFER_TYPE;
    pstSmsCtx->stCnmiType.CnmiMtType      = AT_CNMI_MT_NO_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiBmType      = AT_CNMI_BM_NO_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiDsType      = AT_CNMI_DS_NO_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiBfrType     = AT_CNMI_BFR_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiTmpModeType = AT_CNMI_MODE_BUFFER_TYPE;
    pstSmsCtx->stCnmiType.CnmiTmpMtType   = AT_CNMI_MT_NO_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiTmpBmType   = AT_CNMI_BM_NO_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiTmpDsType   = AT_CNMI_DS_NO_SEND_TYPE;
    pstSmsCtx->stCnmiType.CnmiTmpBfrType  = AT_CNMI_BFR_SEND_TYPE;

    /* 发送域初始化 */
    pstSmsCtx->stCgsmsSendDomain.ucActFlg     = VOS_FALSE;
    pstSmsCtx->stCgsmsSendDomain.enSendDomain = MN_MSG_SEND_DOMAIN_CS_PREFERRED;

    /* 短信接收定制类型初始化*/
    pstSmsCtx->stSmMeFullCustomize.ucActFlg      = VOS_FALSE;
    pstSmsCtx->stSmMeFullCustomize.enMtCustomize = MN_MSG_MT_CUSTOMIZE_NONE;

    /* 文本短信相关参数初始化 */
    /*
    27005 3 Text Mode 3.1 Parameter Definitions
    Message Data Parameters
    <fo> depending on the command or result code: first octet of 3GPP TS 23.040
    [3] SMS-DELIVER, SMS-SUBMIT (default 17), SMS-STATUS-REPORT, or SMS-COMMAND
    (default 2) in integer format
    <vp> depending on SMS-SUBMIT <fo> setting: 3GPP TS 23.040 [3] TP-Validity-
    Period either in integer format (default 167), in time-string format (refer
    <dt>), or if EVPF is supported, in enhanced format (hexadecimal coded string
    with double quotes)
    */
    TAF_MEM_SET_S(&(pstSmsCtx->stCscaCsmpInfo), sizeof(pstSmsCtx->stCscaCsmpInfo), 0x00, sizeof(pstSmsCtx->stCscaCsmpInfo));
    pstSmsCtx->stCscaCsmpInfo.stParmInUsim.ucParmInd = 0xff;
    pstSmsCtx->stCscaCsmpInfo.stVp.enValidPeriod     =
    MN_MSG_VALID_PERIOD_RELATIVE;
    pstSmsCtx->stCscaCsmpInfo.stVp.u.ucOtherTime     = AT_CSMP_SUBMIT_VP_DEFAULT_VALUE;

    pstSmsCtx->stCscaCsmpInfo.ucDefaultSmspIndex     = AT_CSCA_CSMP_STORAGE_INDEX;

    /* 短信及状态报告读，删除，写，发送或接收存储介质初始化 */
    pstSmsCtx->stCpmsInfo.stRcvPath.enStaRptMemStore = MN_MSG_MEM_STORE_SIM;
    pstSmsCtx->stCpmsInfo.stRcvPath.enSmMemStore     = MN_MSG_MEM_STORE_SIM;
    pstSmsCtx->stCpmsInfo.enMemReadorDelete          = MN_MSG_MEM_STORE_SIM;
    pstSmsCtx->stCpmsInfo.enMemSendorWrite           = MN_MSG_MEM_STORE_SIM;

    /* 短信及状态报告接收上报方式初始化 */
    pstSmsCtx->stCpmsInfo.stRcvPath.enRcvSmAct       = MN_MSG_RCVMSG_ACT_STORE;
    pstSmsCtx->stCpmsInfo.stRcvPath.enRcvStaRptAct   = MN_MSG_RCVMSG_ACT_STORE;
    pstSmsCtx->stCpmsInfo.stRcvPath.enSmsServVersion = MN_MSG_CSMS_MSG_VERSION_PHASE2_PLUS;

    /* 短信或状态报告不存储直接上报PDU的缓存初始化 */
    TAF_MEM_SET_S(&(pstSmsCtx->stSmtBuffer), sizeof(pstSmsCtx->stSmtBuffer), 0x00, sizeof(pstSmsCtx->stSmtBuffer));

    /* 短信自动应答缓存数据指针初始化 */
    TAF_MEM_SET_S(pstSmsCtx->astSmsMtBuffer,
               sizeof(pstSmsCtx->astSmsMtBuffer),
               0x00,
               (sizeof(AT_SMS_MT_BUFFER_STRU) * AT_SMSMT_BUFFER_MAX));

    /* 广播短信的语言选择和不存储直接上报PDU的缓存初始化 */
    TAF_MEM_SET_S(&(pstSmsCtx->stCbsDcssInfo), sizeof(pstSmsCtx->stCbsDcssInfo), 0x00, sizeof(pstSmsCtx->stCbsDcssInfo));
    TAF_MEM_SET_S(&(pstSmsCtx->stCbmBuffer), sizeof(pstSmsCtx->stCbmBuffer), 0x00, sizeof(pstSmsCtx->stCbmBuffer));

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemNetCtx
 功能描述  : 初始化网卡相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月21日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_InitModemNetCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    pstNetCtx = AT_GetModemNetCtxAddrFromModemId(enModemId);

    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-1, begin */
    /* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-1, end */
    pstNetCtx->ucCerssiReportType      = AT_CERSSI_REPORT_TYPE_5DB_CHANGE_REPORT;
    pstNetCtx->ucCregType              = AT_CREG_RESULT_CODE_NOT_REPORT_TYPE;
    pstNetCtx->ucCgregType             = AT_CGREG_RESULT_CODE_NOT_REPORT_TYPE;

    pstNetCtx->ucCopsFormatType         = AT_COPS_LONG_ALPH_TYPE;
    pstNetCtx->enPrefPlmnType           = MN_PH_PREF_PLMN_UPLMN;
    pstNetCtx->ucCpolFormatType         = AT_COPS_NUMERIC_TYPE;
    pstNetCtx->ucRoamFeature            = AT_ROAM_FEATURE_OFF;
    pstNetCtx->ucSpnType                = 0;
    pstNetCtx->ucCerssiMinTimerInterval = 0;
    pstNetCtx->enCalculateAntennaLevel  = AT_CMD_ANTENNA_LEVEL_0;
    TAF_MEM_SET_S(pstNetCtx->aenAntennaLevel, sizeof(pstNetCtx->aenAntennaLevel), 0x00, sizeof(pstNetCtx->aenAntennaLevel));
    TAF_MEM_SET_S(&(pstNetCtx->stTimeInfo), sizeof(pstNetCtx->stTimeInfo), 0x00, sizeof(pstNetCtx->stTimeInfo));

    pstNetCtx->ucCeregType           = AT_CEREG_RESULT_CODE_NOT_REPORT_TYPE;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemAgpsCtx
 功能描述  : 初始化AGPS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月21日
    作    者   : l60609
    修改内容   : 新生成函数

  2.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : LCS特性

*****************************************************************************/
VOS_VOID AT_InitModemAgpsCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_AGPS_CTX_STRU             *pstAgpsCtx = VOS_NULL_PTR;

    pstAgpsCtx = AT_GetModemAgpsCtxAddrFromModemId(enModemId);

    TAF_MEM_SET_S(&(pstAgpsCtx->stXml), sizeof(pstAgpsCtx->stXml), 0x00, sizeof(pstAgpsCtx->stXml));

    pstAgpsCtx->enCposrReport   = AT_CPOSR_DISABLE;
    pstAgpsCtx->enXcposrReport  = AT_XCPOSR_DISABLE;
    pstAgpsCtx->enCmolreType    = AT_CMOLRE_NUMERIC;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemPsCtx
 功能描述  : 初始化PS域和modem相关的上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : l60609
    修改内容   : 新生成函数

  2.日    期   : 2013年11月11日
    作    者   : A00165503
    修改内容   : DTS2013110900839: 增加承载IP与RABID映射的初始化
*****************************************************************************/
VOS_VOID AT_InitModemPsCtx(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt;

    pstPsCtx = AT_GetModemPsCtxAddrFromModemId(enModemId);

    /* 初始化CID和呼叫实体映射表 */
    for (ulCnt = 0; ulCnt <= TAF_MAX_CID; ulCnt++)
    {
        pstPsCtx->aucCidToIndexTbl[ulCnt] = AT_PS_CALL_INVALID_CALLID;
    }

    /* 初始化呼叫实体 */
    for (ulCnt = 0; ulCnt < AT_PS_MAX_CALL_NUM; ulCnt++)
    {
        pstPsCtx->astCallEntity[ulCnt].ulUsedFlg   = VOS_FALSE;

        TAF_MEM_SET_S(&pstPsCtx->astCallEntity[ulCnt].stUsrDialParam,
                   sizeof(pstPsCtx->astCallEntity[ulCnt].stUsrDialParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));

        pstPsCtx->astCallEntity[ulCnt].ucIpv4Cid   = AT_PS_CALL_INVALID_CID;
        pstPsCtx->astCallEntity[ulCnt].enIpv4State = AT_PDP_STATE_IDLE;

        TAF_MEM_SET_S(&pstPsCtx->astCallEntity[ulCnt].stIpv4DhcpInfo,
                   sizeof(pstPsCtx->astCallEntity[ulCnt].stIpv4DhcpInfo), 0x00, sizeof(AT_IPV4_DHCP_PARAM_STRU));

        pstPsCtx->astCallEntity[ulCnt].ucIpv6Cid   = AT_PS_CALL_INVALID_CID;
        pstPsCtx->astCallEntity[ulCnt].enIpv6State = AT_PDP_STATE_IDLE;

        TAF_MEM_SET_S(&pstPsCtx->astCallEntity[ulCnt].stIpv6RaInfo,
                   sizeof(pstPsCtx->astCallEntity[ulCnt].stIpv6RaInfo), 0x00, sizeof(AT_IPV6_RA_INFO_STRU));
        TAF_MEM_SET_S(&pstPsCtx->astCallEntity[ulCnt].stIpv6DhcpInfo,
                   sizeof(pstPsCtx->astCallEntity[ulCnt].stIpv6DhcpInfo), 0x00, sizeof(AT_IPV6_DHCP_PARAM_STRU));
    }

    /* 初始化CHDATA CFG */
    for (ulCnt = 0; ulCnt <= TAF_MAX_CID; ulCnt++)
    {
        pstPsCtx->astChannelCfg[ulCnt].ulUsed        = VOS_FALSE;
        pstPsCtx->astChannelCfg[ulCnt].ulRmNetId     = AT_PS_INVALID_RMNET_ID;
        pstPsCtx->astChannelCfg[ulCnt].ulRmNetActFlg = VOS_FALSE;
    }

    /* 初始化错误码 */
    pstPsCtx->enPsErrCause = TAF_PS_CAUSE_SUCCESS;

    /* 初始化IP地址与RABID的映射表 */
    TAF_MEM_SET_S(pstPsCtx->aulIpAddrRabIdMap, sizeof(pstPsCtx->aulIpAddrRabIdMap), 0x00, (sizeof(VOS_UINT32) * AT_PS_RABID_MAX_NUM));

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemPrivacyFilterCtx
 功能描述  : 初始化隐私过滤相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年07月02日
    作    者   : f00179208
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitModemPrivacyFilterCtx(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    AT_MODEM_PRIVACY_FILTER_CTX_STRU   *pstFilterCtx = VOS_NULL_PTR;

    pstFilterCtx = AT_GetModemPrivacyFilterCtxAddrFromModemId(enModemId);

    TAF_MEM_SET_S(pstFilterCtx,
                  (VOS_UINT32)sizeof(AT_MODEM_PRIVACY_FILTER_CTX_STRU),
                  0,
                  (VOS_UINT32)sizeof(AT_MODEM_PRIVACY_FILTER_CTX_STRU));

    pstFilterCtx->ucFilterEnableFlg = VOS_FALSE;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemCdmaModemSwitchCtx
 功能描述  : 初始化CDMAMODEMSWITCH上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年10月27日
    作    者   : h00313353
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitModemCdmaModemSwitchCtx(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    AT_MODEM_CDMAMODEMSWITCH_CTX_STRU  *pstCdmaModemSwitchCtx;

    pstCdmaModemSwitchCtx = AT_GetModemCdmaModemSwitchCtxAddrFromModemId(enModemId);

    TAF_MEM_SET_S(pstCdmaModemSwitchCtx,
                  (VOS_UINT32)sizeof(AT_MODEM_CDMAMODEMSWITCH_CTX_STRU),
                  0x00,
                  (VOS_UINT32)sizeof(AT_MODEM_CDMAMODEMSWITCH_CTX_STRU));

    pstCdmaModemSwitchCtx->ucEnableFlg = VOS_FALSE;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitModemImsCtx
 功能描述  : 初始化IMS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年7月18日
    作    者   : w00316404
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_InitModemImsCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_MODEM_IMS_CONTEXT_STRU           *pstImsCtx = VOS_NULL_PTR;

    pstImsCtx = AT_GetModemImsCtxAddrFromModemId(enModemId);

    TAF_MEM_SET_S(pstImsCtx, sizeof(AT_MODEM_IMS_CONTEXT_STRU), 0x00, sizeof(AT_MODEM_IMS_CONTEXT_STRU));

    pstImsCtx->stBatteryInfo.enCurrBatteryInfo = AT_IMSA_BATTERY_STATUS_BUTT;
    pstImsCtx->stBatteryInfo.enTempBatteryInfo = AT_IMSA_BATTERY_STATUS_BUTT;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitClientConfiguration
 功能描述  : 初始化AT的Client配置信息
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月13日
    作    者   : l60609
    修改内容   : DSDA Phase II: 新生成函数

*****************************************************************************/
VOS_VOID AT_InitClientConfiguration(VOS_VOID)
{
    VOS_UINT8                           i;
    AT_CLIENT_CTX_STRU                 *pstClientCtx = VOS_NULL_PTR;

    for (i = 0; i < AT_CLIENT_BUTT; i++)
    {
        pstClientCtx = AT_GetClientCtxAddr(i);

        pstClientCtx->stClientConfiguration.ucReportFlg = VOS_TRUE;
        pstClientCtx->stClientConfiguration.enModemId   = MODEM_ID_0;
    }

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitResetCtx
 功能描述  : 初始化RNIC复位信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2013年04月15日
   作    者   : f00179208
   修改内容   : 新生成函数
 2.日    期   : 2013年07月222日
   作    者   : j00177245
   修改内容   : 清理编译warning
*****************************************************************************/
VOS_VOID AT_InitResetCtx(VOS_VOID)
{
    AT_RESET_CTX_STRU                   *pstResetCtx = VOS_NULL_PTR;

    TAF_MEM_SET_S(&g_stAtStatsInfo, (VOS_UINT32)sizeof(g_stAtStatsInfo), 0x00, (VOS_UINT32)sizeof(g_stAtStatsInfo));

    pstResetCtx = AT_GetResetCtxAddr();

    pstResetCtx->hResetSem     = VOS_NULL_PTR;
    pstResetCtx->ulResetingFlag = VOS_FALSE;

    /* 分配二进制信号量 */
    if (VOS_OK != VOS_SmBCreate( "AT", 0, VOS_SEMA4_FIFO, &pstResetCtx->hResetSem))
    {
        (VOS_VOID)vos_printf("Create AT acpu cnf sem failed!\r\n");
        AT_DBG_SET_SEM_INIT_FLAG(VOS_FALSE);
        AT_DBG_CREATE_BINARY_SEM_FAIL_NUM(1);

        return;
    }
    else
    {
        AT_DBG_SAVE_BINARY_SEM_ID(pstResetCtx->hResetSem);
    }

    AT_DBG_SET_SEM_INIT_FLAG(VOS_TRUE);

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitReleaseInfo
 功能描述  : 初始化接入层版本号
 输入参数  : 无
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2016年11月23日
   作    者   : wx270776
   修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitReleaseInfo(VOS_VOID)
{
    TAF_MEM_SET_S(&g_stReleaseInfo, (VOS_UINT32)sizeof(g_stReleaseInfo), 0x00, (VOS_UINT32)sizeof(g_stReleaseInfo));

    g_stReleaseInfo.enAccessStratumRel = AT_ACCESS_STRATUM_REL9;

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitCommCtx
 功能描述  : 初始化公共的上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

  2.日    期   : 2013年5月2日
    作    者   : l60609
    修改内容   : IPv6&TAF/SM Project

  3.日    期   : 2013年09月22日
    作    者   : j00174725
    修改内容   : UART-MODEM: 增加UART上下文初始化

  4.日    期   : 2015年3月12日
    作    者   : A00165503
    修改内容   : DTS2015032409785: 增加水线检测功能
*****************************************************************************/
VOS_VOID AT_InitCommCtx(VOS_VOID)
{
    AT_COMM_CTX_STRU                   *pstCommCtx = VOS_NULL_PTR;

    pstCommCtx = AT_GetCommCtxAddr();

    /* 默认应该类型为MP */
    pstCommCtx->ucSystemAppConfigAddr = SYSTEM_APP_MP;

    /* 初始化MUX相关的上下文 */
    TAF_MEM_SET_S(&(pstCommCtx->stMuxCtx), (VOS_UINT32)sizeof(pstCommCtx->stMuxCtx), 0x00, (VOS_UINT32)sizeof(AT_MUX_CTX_STRU));

    /* 初始化PS域公共的上下文 */
    AT_InitCommPsCtx();

    AT_InitCommPbCtx();

    AT_InitCmdProcCtx();

    AT_InitReleaseInfo();

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitClientCtx
 功能描述  : 初始化通道相关的上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月24日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_InitClientCtx(VOS_VOID)
{
    AT_InitClientConfiguration();
}

/*****************************************************************************
 函 数 名  : AT_InitModemCtx
 功能描述  : 初始化Modem相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16 enModemId
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月24日
    作    者   : l60609
    修改内容   : 新生成函数
  2.日    期   : 2012年12月27日
    作    者   : z00220246
    修改内容   : 初始化平台默认接入技术
  3.日    期   : 2013年3月5日
    作    者   : l60609
    修改内容   : DSDA PHASE III
  4.日    期   : 2013年5月2日
    作    者   : l60609
    修改内容   : IPv6&TAF/SM Project
*****************************************************************************/
VOS_VOID AT_InitModemCtx(MODEM_ID_ENUM_UINT16 enModemId)
{
    AT_InitUsimStatus(enModemId);

    AT_InitPlatformRatList(enModemId);

    AT_InitModemCcCtx(enModemId);

    AT_InitModemSsCtx(enModemId);

    AT_InitModemSmsCtx(enModemId);

    AT_InitModemNetCtx(enModemId);

    AT_InitModemAgpsCtx(enModemId);

    AT_InitModemPsCtx(enModemId);

    AT_InitModemImsCtx(enModemId);

    AT_InitModemPrivacyFilterCtx(enModemId);

    AT_InitModemCdmaModemSwitchCtx(enModemId);

    return;
}

/*****************************************************************************
 函 数 名  : AT_InitCtx
 功能描述  : 初始化AT的上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月13日
    作    者   : l60609
    修改内容   : DSDA Phase II: 新生成函数
  2.日    期   : 2013年3月4日
    作    者   : l60609
    修改内容   : DSDA PHASE III
*****************************************************************************/
VOS_VOID AT_InitCtx(VOS_VOID)
{
    MODEM_ID_ENUM_UINT16                enModemId;

    AT_InitCommCtx();

    for (enModemId = 0; enModemId < MODEM_ID_BUTT; enModemId++)
    {
        AT_InitModemCtx(enModemId);
    }

    AT_InitClientCtx();

    return;
}

/*****************************************************************************
 函 数 名  : AT_GetModemIDFromPid
 功能描述  : 根据PID得到ModemID
 输入参数  : VOS_UINT32
 输出参数  : 无
 返 回 值  : MODEM_ID_ENUM_UINT16
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年08月08日
    作    者   : w00316404
    修改内容   : 新生成函数
*****************************************************************************/
MODEM_ID_ENUM_UINT16 AT_GetModemIDFromPid(VOS_UINT32 ulPid)
{
    VOS_UINT32                          ulModemPidTabLen;
    VOS_UINT32                          i;

    ulModemPidTabLen    = (sizeof(g_astAtModemPidTab)/sizeof(AT_MODEM_PID_TAB_STRU));

    for (i = 0; i < ulModemPidTabLen; i++)
    {

        if (ulPid == g_astAtModemPidTab[i].ulModem0Pid)
        {
            return MODEM_ID_0;
        }

        if (ulPid == g_astAtModemPidTab[i].ulModem1Pid)
        {
            return MODEM_ID_1;
        }


    }

    return MODEM_ID_BUTT;
}


/*****************************************************************************
 函 数 名  : AT_GetSystemAppConfigAddr
 功能描述  : 获取APP控制特性的NV的地址
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 控制特性的信息
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2011年12月07日
   作    者   : s62952
   修改内容   : 新生成函数
 2.日    期   : 2013年3月5日
   作    者   : l60609
   修改内容   : DSDA PHASE III
*****************************************************************************/
VOS_UINT8* AT_GetSystemAppConfigAddr(VOS_VOID)
{
    AT_COMM_CTX_STRU                   *pstCommCtx = VOS_NULL_PTR;

    pstCommCtx = AT_GetCommCtxAddr();

    return &(pstCommCtx->ucSystemAppConfigAddr);
}

/*****************************************************************************
 函 数 名  : AT_GetResetCtxAddr
 功能描述  : 获取复位上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_RESET_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年04月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_RESET_CTX_STRU* AT_GetResetCtxAddr(VOS_VOID)
{
    return &(g_stAtResetCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetCommCtxAddr
 功能描述  : 获取公共上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_COMM_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_COMM_CTX_STRU* AT_GetCommCtxAddr(VOS_VOID)
{
    return &(g_stAtCommCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetCommPsCtxAddr
 功能描述  : 获取PS域的公共上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_PS_COMM_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月25日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_COMM_PS_CTX_STRU* AT_GetCommPsCtxAddr(VOS_VOID)
{
    return &(g_stAtCommCtx.stPsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetCommPbCtxAddr
 功能描述  : 获取电话本公共上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_COMM_PB_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月12日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
AT_COMM_PB_CTX_STRU* AT_GetCommPbCtxAddr(VOS_VOID)
{
    return &(g_stAtCommCtx.stCommPbCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetMsgNumCtrlCtxAddr
 功能描述  : 获取消息数目控制上下文地址
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_CMD_MSG_NUM_CTRL_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年9月27日
    作    者   : m00217266
    修改内容   : 新生成函数
*****************************************************************************/
AT_CMD_MSG_NUM_CTRL_STRU* AT_GetMsgNumCtrlCtxAddr(VOS_VOID)
{
    return &(g_stAtCommCtx.stMsgNumCtrlCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemCtxAddr
 功能描述  : 获取Modem相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_CTX_STRU* AT_GetModemCtxAddr(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId]);
}

/*****************************************************************************
 函 数 名  : AT_GetUsimInfoCtxFromModemId
 功能描述  : 根据MODEM ID获取卡状态
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_USIM_INFO_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月5日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_USIM_INFO_CTX_STRU* AT_GetUsimInfoCtxFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stAtUsimInfoCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetSptRatFromModemId
 功能描述  : 根据MODEM ID获取平台能力
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_SPT_RAT_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月5日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_SPT_RAT_STRU* AT_GetSptRatFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stPlatformCapList.stPlatformRatList);
}

/* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-1, begin */

/* Deleted by s00217060 for 主动上报AT命令控制下移至C核, 2013-4-1, end */

/*****************************************************************************
 函 数 名  : AT_GetModemCcCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取呼叫相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_CC_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_CC_CTX_STRU* AT_GetModemCcCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stCcCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemCcCtxAddrFromClientId
 功能描述  : 根据ClientId返回呼叫相关的上下文
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : AT_MODEM_CC_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_CC_CTX_STRU* AT_GetModemCcCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemCcCtxAddrFromClientId: Get modem id fail.");
    }

    return &(g_astAtModemCtx[enModemId].stCcCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemSsCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取SS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_SS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_SS_CTX_STRU* AT_GetModemSsCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stSsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemSsCtxAddrFromClientId
 功能描述  : 根据ClientId返回补充业务相关的上下文
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : AT_MODEM_SS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_SS_CTX_STRU* AT_GetModemSsCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemSsCtxAddrFromClientId: Get modem id fail.");
    }

    return &(g_astAtModemCtx[enModemId].stSsCtx);
}
/*****************************************************************************
 函 数 名  : AT_GetModemSmsCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取短信相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_SMS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_SMS_CTX_STRU* AT_GetModemSmsCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stSmsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemSmsCtxAddrFromClientId
 功能描述  : 根据ClientId获取短消息相关的上下文
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : AT_MODEM_SMS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月22日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_SMS_CTX_STRU* AT_GetModemSmsCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemSmsCtxAddrFromClientId: Get modem id fail");
    }

    return &(g_astAtModemCtx[enModemId].stSmsCtx);
}


/*****************************************************************************
 函 数 名  : AT_GetModemNetCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取网络相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_NET_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_NET_CTX_STRU* AT_GetModemNetCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stNetCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemNetCtxAddrFromClientId
 功能描述  : 根据ClientId获取网络相关的上下文
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : AT_MODEM_NET_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月22日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_NET_CTX_STRU* AT_GetModemNetCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemNetCtxAddrFromClientId: Get modem id fail");
    }

    return &(g_astAtModemCtx[enModemId].stNetCtx);
}
/*****************************************************************************
 函 数 名  : AT_GetModemAgpsCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取AGPS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_AGPS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月21日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_AGPS_CTX_STRU* AT_GetModemAgpsCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stAgpsCtx);
}
/*****************************************************************************
 函 数 名  : AT_GetModemAgpsCtxAddrFromClientId
 功能描述  : 根据ClientId值获取AGPS相关的上下文
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : AT_MODEM_AGPS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月21日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_AGPS_CTX_STRU* AT_GetModemAgpsCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemAgpsCtxAddrFromClientId: Get modem id fail.");
    }

    return &(g_astAtModemCtx[enModemId].stAgpsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemPsCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取PS域MODEM相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_PS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月25日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_PS_CTX_STRU* AT_GetModemPsCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stPsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemPsCtxAddrFromClientId
 功能描述  : 根据CLIENT ID获取PS域MODEM相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_PS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月25日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_PS_CTX_STRU* AT_GetModemPsCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemPsCtxAddrFromClientId: Get modem id fail.");
    }

    return &(g_astAtModemCtx[enModemId].stPsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemImsCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取IMS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_IMS_CONTEXT_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年7月18日
    作    者   : w00316404
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_IMS_CONTEXT_STRU* AT_GetModemImsCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stAtImsCtx);
}
/*****************************************************************************
 函 数 名  : AT_GetModemImsCtxAddrFromClientId
 功能描述  : 根据ClientId值获取IMS相关的上下文
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : AT_MODEM_IMS_CONTEXT_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年7月18日
    作    者   : w00316404
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_IMS_CONTEXT_STRU* AT_GetModemImsCtxAddrFromClientId(
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRslt;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_GetModemImsCtxAddrFromClientId: Get modem id fail.");
    }

    return &(g_astAtModemCtx[enModemId].stAtImsCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetClientCtxAddr
 功能描述  : 获取端口相关的上下文
 输入参数  : AT_CLIENT_ID_ENUM_UINT16            enClientId
 输出参数  : 无
 返 回 值  : AT_CLIENT_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月20日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
AT_CLIENT_CTX_STRU* AT_GetClientCtxAddr(
    AT_CLIENT_ID_ENUM_UINT16            enClientId
)
{
    return &(g_astAtClientCtx[enClientId]);
}

/*****************************************************************************
 函 数 名  : AT_GetModemIdFromClient
 功能描述  : 根据AT CLIENT INDEX获取归属的MODEM ID
 输入参数  : VOS_UINT16                          usClientId
             MODEM_ID_ENUM_UINT16               *pModemId
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月13日
    作    者   : l60609
    修改内容   : 新生成函数

  2.日    期   : 2013年03月13日
    作    者   : f00179208
    修改内容   : DSDA PHASE III:增加异常保护

  3.日    期   : 2015年5月27日
    作    者   : l00198894
    修改内容   : TSTS
*****************************************************************************/
VOS_UINT32 AT_GetModemIdFromClient(
    VOS_UINT16                          usClientId,
    MODEM_ID_ENUM_UINT16               *pModemId
)
{
    AT_CLIENT_CTX_STRU                 *pstAtClientCtx = VOS_NULL_PTR;

    /* 判断是否是MODEM0广播的client index */
    if ((AT_BROADCAST_CLIENT_INDEX_MODEM_0 == usClientId)
     || (AT_BROADCAST_CLIENT_ID_MODEM_0 == usClientId))
    {
        *pModemId = MODEM_ID_0;
    }
    /* 判断是否是MODEM1广播的client index */
    else if ((AT_BROADCAST_CLIENT_INDEX_MODEM_1 == usClientId)
          || (AT_BROADCAST_CLIENT_ID_MODEM_1 == usClientId))
    {
        *pModemId = MODEM_ID_1;
    }
    /* 判断是否是MODEM2广播的client index */
    else if ((AT_BROADCAST_CLIENT_INDEX_MODEM_2 == usClientId)
          || (AT_BROADCAST_CLIENT_ID_MODEM_2 == usClientId))
    {
        *pModemId = MODEM_ID_2;
    }
    /* 非广播client index */
    else
    {
        /* client index 无效，直接返回 */
        if (usClientId >= AT_CLIENT_BUTT)
        {
            return VOS_ERR;
        }

        pstAtClientCtx = AT_GetClientCtxAddr(usClientId);

        *pModemId = pstAtClientCtx->stClientConfiguration.enModemId;
    }

    /* 在单卡的时候NV里读出来的MODEMID为MODEM1时，会发生内存越界，此处做个异常保护 */
    if (*pModemId >= MODEM_ID_BUTT)
    {
        AT_ERR_LOG("AT_GetModemIdFromClient: modem id is invalid");

        *pModemId = MODEM_ID_0;
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_GetDestPid
 功能描述  : 获取实际发送的PID
 输入参数  : MN_CLIENT_ID_T                      usClientId
             VOS_UINT32                          ulRcvPid
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月17日
    作    者   : l60609
    修改内容   : 新生成函数

  2.日    期   : 2015年5月27日
    作    者   : l00198894
    修改内容   : TSTS
*****************************************************************************/
VOS_UINT32 AT_GetDestPid(
    MN_CLIENT_ID_T                      usClientId,
    VOS_UINT32                          ulRcvPid
)
{
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          i;

    enModemId = MODEM_ID_0;

    /* 获取client id对应的Modem Id */
    ulRslt = AT_GetModemIdFromClient(usClientId, &enModemId);

    /* modem 1上ulRcvPid对应的pid */
    if ((VOS_OK == ulRslt)
     && (MODEM_ID_0 != enModemId))
    {
        for (i = 0; i < (sizeof(g_astAtModemPidTab)/sizeof(AT_MODEM_PID_TAB_STRU)); i++)
        {
            if (ulRcvPid != g_astAtModemPidTab[i].ulModem0Pid)
            {
                continue;
            }

            if (MODEM_ID_1 == enModemId)
            {
                return g_astAtModemPidTab[i].ulModem1Pid;
            }

            if (MODEM_ID_2 == enModemId)
            {
                return g_astAtModemPidTab[i].ulModem2Pid;
            }
        }

        /* 如果出现找不到对应的PID应该时出错了 */
        if (i >= (sizeof(g_astAtModemPidTab)/sizeof(AT_MODEM_PID_TAB_STRU)))
        {
            (VOS_VOID)vos_printf("AT_GetDestPid: usClientId is %d, ulRcvPid is %d no modem1 pid. \r\n", usClientId, ulRcvPid);
        }
    }

    return ulRcvPid;
}

/*****************************************************************************
 函 数 名  : AT_GetRealClientId
 功能描述  : 获取对应Modem的CilentId
 输入参数  : MN_CLIENT_ID_T                      usClientId
             VOS_UINT32                          ulPid
 输出参数  : 无
 返 回 值  : MN_CLIENT_ID_T
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月1日
    作    者   : L60609
    修改内容   : 新生成函数

  2.日    期   : 2015年5月28日
    作    者   : l00198894
    修改内容   : TSTS
*****************************************************************************/
MN_CLIENT_ID_T AT_GetRealClientId(
    MN_CLIENT_ID_T                      usClientId,
    VOS_UINT32                          ulPid
)
{
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 根据Pid的值获取ModemId */
    enModemId = AT_GetModemIDFromPid(ulPid);

    /* 根据ModemId获取ClientId */
    if (MODEM_ID_0 == enModemId)
    {
        return (usClientId & AT_BROADCAST_CLIENT_ID_MODEM_0);
    }

    if (MODEM_ID_1 == enModemId)
    {
        return (usClientId & AT_BROADCAST_CLIENT_ID_MODEM_1);
    }


    AT_ERR_LOG1("AT_GetRealClientId, enModemId err", enModemId);

    return usClientId;
}

/*****************************************************************************
 函 数 名  : AT_IsModemSupportLte
 功能描述  : 判断当前Modem是否支持某种接入技术
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
             TAF_MMA_RAT_TYPE_ENUM_UINT8         enRat
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月27日
    作    者   : z00220246
    修改内容   : 新生成函数
  2.日    期   : 2015年4月10日
    作    者   : h00313353
    修改内容   : SysCfg重构
*****************************************************************************/
VOS_UINT8 AT_IsModemSupportRat(
    MODEM_ID_ENUM_UINT16                enModemId,
    TAF_MMA_RAT_TYPE_ENUM_UINT8         enRat
)
{
    AT_MODEM_SPT_RAT_STRU              *pstSptRatList = VOS_NULL_PTR;

    pstSptRatList = AT_GetSptRatFromModemId(enModemId);
    if (TAF_MMA_RAT_LTE == enRat)
    {
        return pstSptRatList->ucPlatformSptLte;
    }
    if (TAF_MMA_RAT_WCDMA == enRat)
    {
        if ((VOS_TRUE == pstSptRatList->ucPlatformSptWcdma)
         || (VOS_TRUE == pstSptRatList->ucPlatformSptUtralTDD))
        {
            return VOS_TRUE;
        }
        else
        {
            return VOS_FALSE;
        }
    }
    if (TAF_MMA_RAT_GSM == enRat)
    {
        return pstSptRatList->ucPlatformSptGsm;
    }

    return VOS_FALSE;
}

/*****************************************************************************
 函 数 名  : AT_IsModemSupportUtralTDDRat
 功能描述  : 判断当前Modem是否支持UTRALTDD
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : VOS_UINT8
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年8月22日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT8 AT_IsModemSupportUtralTDDRat(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    AT_MODEM_SPT_RAT_STRU              *pstSptRatList = VOS_NULL_PTR;

    pstSptRatList = AT_GetSptRatFromModemId(enModemId);

    return pstSptRatList->ucPlatformSptUtralTDD;
}


/*****************************************************************************
 函 数 名  : AT_SetCsCallErrCause
 功能描述  : 设置CS域呼叫错误码
 输入参数  : VOS_UINT16                          usClientId
             TAF_CS_CAUSE_ENUM_UINT32            enCsErrCause
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月21日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_SetCsCallErrCause(
    VOS_UINT16                          usClientId,
    TAF_CS_CAUSE_ENUM_UINT32            enCsErrCause
)
{
    AT_MODEM_CC_CTX_STRU               *pstCcCtx = VOS_NULL_PTR;

    pstCcCtx = AT_GetModemCcCtxAddrFromClientId(usClientId);

    pstCcCtx->enCsErrCause = enCsErrCause;

    return;
}

/*****************************************************************************
 函 数 名  : AT_GetCsCallErrCause
 功能描述  : 获取CS域呼叫错误码
 输入参数  : VOS_UINT16                          usClientId
 输出参数  : 无
 返 回 值  : TAF_CS_CAUSE_ENUM_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月21日
    作    者   : l60609
    修改内容   : 新生成函数

*****************************************************************************/
TAF_CS_CAUSE_ENUM_UINT32 AT_GetCsCallErrCause(
    VOS_UINT16                          usClientId
)
{
    AT_MODEM_CC_CTX_STRU               *pstCcCtx = VOS_NULL_PTR;

    pstCcCtx = AT_GetModemCcCtxAddrFromClientId(usClientId);

    return pstCcCtx->enCsErrCause;
}


/*****************************************************************************
 函 数 名  : AT_GetAbortCmdPara
 功能描述  : 获取AT打断命令的参数
 输出参数  : 无
 返 回 值  : AT打断命令的参数
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月22日
    作    者   : lijun 00171473
    修改内容   : 新生成函数 for V7R1C50_At_Abort

  2.日    期   : 2013年10月18日
    作    者   : w00242748
    修改内容   : 封装打断结构体
*****************************************************************************/
AT_ABORT_CMD_PARA_STRU* AT_GetAbortCmdPara(VOS_VOID)
{
    return &(gstAtAbortCmdCtx.stAtAbortCmdPara);
}

/*****************************************************************************
 函 数 名  : AT_GetAbortRspStr
 功能描述  : 获取AT打断的返回结果字符串
 输出参数  : 无
 返 回 值  : AT打断的返回结果字符串, 带有\0结束符的字符串
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月22日
    作    者   : lijun 00171473
    修改内容   : 新生成函数 for V7R1C50_At_Abort

  2.日    期   : 2013年10月18日
    作    者   : w00242748
    修改内容   : 封装打断结构体
*****************************************************************************/
VOS_UINT8* AT_GetAbortRspStr(VOS_VOID)
{
    return (gstAtAbortCmdCtx.stAtAbortCmdPara.aucAbortAtRspStr);
}


/*****************************************************************************
 函 数 名  : AT_GetSsCustomizePara
 功能描述  : 获取指令SS 业务的定制状态
 输入参数  : AT_SS_CUSTOMIZE_TYPE_UINT8 enSsCustomizeType   业务类型
 输出参数  : 无
 返 回 值  : VOS_UINT32
                VOS_FALSE       业务未定制
                VOS_TRUE        业务已定制
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年01月24日
    作    者   : f62575
    修改内容   : DTS2013012408620, 支持SS查询操作输出网络SS-STATUS参数

*****************************************************************************/
VOS_UINT32 AT_GetSsCustomizePara(AT_SS_CUSTOMIZE_TYPE_UINT8 enSsCustomizeType)
{
    VOS_UINT8                           ucMask;

    if (VOS_TRUE != g_stAtSsCustomizePara.ucStatus)
    {
        return VOS_FALSE;
    }

    ucMask = (VOS_UINT8)((VOS_UINT32)AT_SS_CUSTOMIZE_SERVICE_MASK << enSsCustomizeType);
    if (0 != (g_stAtSsCustomizePara.ucSsCmdCustomize & ucMask))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}

/*****************************************************************************
 函 数 名  : AT_GetResetSem
 功能描述  : 获取AT复位信号量
 输入参数  : 无
 输出参数  : 无
 返 回 值  : AT复位信号量
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2013年04月17日
   作    者   : f00179208
   修改内容   : 新生成函数
*****************************************************************************/
VOS_SEM AT_GetResetSem(VOS_VOID)
{
    return g_stAtResetCtx.hResetSem;
}

/*****************************************************************************
 函 数 名  : AT_GetResetFlag
 功能描述  : 获取AT复位标志
 输入参数  : 无
 输出参数  : 无
 返 回 值  : AT复位标志
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2013年04月17日
   作    者   : f00179208
   修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 AT_GetResetFlag(VOS_VOID)
{
    return g_stAtResetCtx.ulResetingFlag;
}

/*****************************************************************************
 函 数 名  : AT_SetResetFlag
 功能描述  : 设置AT复位标志
 输入参数  : 复位状态
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2013年04月17日
   作    者   : f00179208
   修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_SetResetFlag(VOS_UINT32 ulFlag)
{
    g_stAtResetCtx.ulResetingFlag = ulFlag;
    return;
}


/*****************************************************************************
 函 数 名  : AT_GetModemPrivacyFilterCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取隐私过滤相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_PRIVACY_FILTER_CTX_STRU *
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年07月01日
    作    者   : f00179208
    修改内容   : 新生成函数
*****************************************************************************/
AT_MODEM_PRIVACY_FILTER_CTX_STRU* AT_GetModemPrivacyFilterCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stFilterCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemCdmaModemSwitchCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取隐私过滤相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_CDMAMODEMSWITCH_CTX_STRU *
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年10月27日
    作    者   : h00313353
    修改内容   : 新生成函数
*****************************************************************************/
AT_MODEM_CDMAMODEMSWITCH_CTX_STRU* AT_GetModemCdmaModemSwitchCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stCdmaModemSwitchCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetModemMtInfoCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取AGPS相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_MT_INFO_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月30日
    作    者   : z60575
    修改内容   : 新生成函数

*****************************************************************************/
AT_MODEM_MT_INFO_CTX_STRU* AT_GetModemMtInfoCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stMtInfoCtx);
}

/*****************************************************************************
 函 数 名  : AT_InitTraceMsgTab
 功能描述  : 初始化勾包使用的端口号
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月07日
    作    者   : j00174725
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_InitTraceMsgTab(VOS_VOID)
{
    TAF_MEM_SET_S(g_stAtTraceMsgIdTab, sizeof(g_stAtTraceMsgIdTab), 0xFF, sizeof(g_stAtTraceMsgIdTab));
}

/*****************************************************************************
 函 数 名  : AT_GetResultMsgID
 功能描述  : 获取 Result MSG ID
 输入参数  : ucIndex   -- 端口client ID 正确性由调用者保证
 输出参数  : 无
 返 回 值  : AT_INTER_MSG_ID_ENUM_UINT32 -- result msg id
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月07日
    作    者   : j00174725
    修改内容   : 新生成函数

  2.日    期   : 2015年5月27日
    作    者   : l00198894
    修改内容   : TSTS
*****************************************************************************/
AT_INTER_MSG_ID_ENUM_UINT32 AT_GetResultMsgID(VOS_UINT8 ucIndex)
{
    AT_INTER_MSG_ID_ENUM_UINT32         enResultMsgID;

    if (AT_BROADCAST_CLIENT_INDEX_MODEM_0 == ucIndex)
    {
        enResultMsgID = ID_AT_MNTN_RESULT_BROADCAST_MODEM_0;
    }
    else if (AT_BROADCAST_CLIENT_INDEX_MODEM_1 == ucIndex)
    {
        enResultMsgID = ID_AT_MNTN_RESULT_BROADCAST_MODEM_1;
    }
    else if (AT_BROADCAST_CLIENT_INDEX_MODEM_2 == ucIndex)
    {
        enResultMsgID = ID_AT_MNTN_RESULT_BROADCAST_MODEM_2;
    }
    else
    {
        enResultMsgID = g_stAtTraceMsgIdTab[ucIndex].enResultMsgID;
    }

    return enResultMsgID;
}

/*****************************************************************************
 函 数 名  : AT_GetCmdMsgID
 功能描述  : 获取 CMD MSG ID
 输入参数  : ucIndex   -- 端口client ID 正确性由调用者保证
 输出参数  : 无
 返 回 值  : AT_INTER_MSG_ID_ENUM_UINT32 --cmd msg ID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月07日
    作    者   : j00174725
    修改内容   : 新生成函数
*****************************************************************************/
AT_INTER_MSG_ID_ENUM_UINT32 AT_GetCmdMsgID(VOS_UINT8 ucIndex)
{
    return g_stAtTraceMsgIdTab[ucIndex].enCmdMsgID;
}

/*****************************************************************************
 函 数 名  : AT_ConfigTraceMsg
 功能描述  : 配置端口与命令消息映射
 输入参数  : ucIndex       - 端口索引
             enCmdMsgId    - 端口命令消息
             enResultMsgId - 端口命令结果
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月07日
    作    者   : j00174725
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID AT_ConfigTraceMsg(
    VOS_UINT8                           ucIndex,
    AT_INTER_MSG_ID_ENUM_UINT32         enCmdMsgId,
    AT_INTER_MSG_ID_ENUM_UINT32         enResultMsgId
)
{
    g_stAtTraceMsgIdTab[ucIndex].enCmdMsgID = enCmdMsgId;
    g_stAtTraceMsgIdTab[ucIndex].enResultMsgID = enResultMsgId;

    return;
}

/*****************************************************************************
 函 数 名  : At_SetAtCmdAbortTickInfo
 功能描述  : 保存设置AT命令时的tick值和使能值
 输入参数  : ucIndex   : 通道索引
             ulTick    : tick值
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月18日
    作    者   : w00242748
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID At_SetAtCmdAbortTickInfo(
    VOS_UINT8                           ucIndex,
    VOS_UINT32                          ulTick
)
{
    gstAtAbortCmdCtx.stCmdAbortTick.ulAtSetTick[ucIndex]    = ulTick;

    return;
}

/*****************************************************************************
 函 数 名  : At_GetAtCmdAbortTickInfo
 功能描述  : 获取保存的设置AT命令时的Tick值信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : AT_CMD_ABORT_TICK_INFO 信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月18日
    作    者   : w00242748
    修改内容   : 新生成函数

*****************************************************************************/
AT_CMD_ABORT_TICK_INFO* At_GetAtCmdAbortTickInfo(VOS_VOID)
{
    return &(gstAtAbortCmdCtx.stCmdAbortTick);
}

/*****************************************************************************
 函 数 名  : At_GetAtCmdAnyAbortFlg
 功能描述  : 获取任意字符打断标记
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 返回任意打断标记标示符
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月18日
    作    者   : w00242748
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT8 At_GetAtCmdAnyAbortFlg(VOS_VOID)
{
    return (gstAtAbortCmdCtx.stAtAbortCmdPara.ucAnyAbortFlg);
}

/*****************************************************************************
 函 数 名  : At_SetAtCmdAnyAbortFlg
 功能描述  : 设置任意字符打断标记
 输入参数  : ucFlg    : 打断标志
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月18日
    作    者   : w00242748
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID At_SetAtCmdAnyAbortFlg(
    VOS_UINT8                           ucFlg
)
{
    gstAtAbortCmdCtx.stAtAbortCmdPara.ucAnyAbortFlg = ucFlg;

    return;
}

/*****************************************************************************
 函 数 名  : AT_GetUartCtxAddr
 功能描述  : 获取UART口公共上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_COMM_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : j00174725
    修改内容   : 新生成函数
*****************************************************************************/
AT_UART_CTX_STRU* AT_GetUartCtxAddr(VOS_VOID)
{
    return &(g_stAtCommCtx.stUartCtx);
}

/*****************************************************************************
 函 数 名  : AT_GetUartRiCfgInfo
 功能描述  : 获取UART物理参数配置信息
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_UART_PHY_CFG_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
AT_UART_PHY_CFG_STRU* AT_GetUartPhyCfgInfo(VOS_VOID)
{
    return &(AT_GetUartCtxAddr()->stPhyConfig);
}

/*****************************************************************************
 函 数 名  : AT_GetUartLineCtrlInfo
 功能描述  : 获取UART端口管脚信号控制信息
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_UART_LINE_CTRL_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
AT_UART_LINE_CTRL_STRU* AT_GetUartLineCtrlInfo(VOS_VOID)
{
    return &(AT_GetUartCtxAddr()->stLineCtrl);
}

/*****************************************************************************
 函 数 名  : AT_GetUartFlowCtrlInfo
 功能描述  : 获取UART端口流控配置信息
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_UART_FLOW_CTRL_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
AT_UART_FLOW_CTRL_STRU* AT_GetUartFlowCtrlInfo(VOS_VOID)
{
    return &(AT_GetUartCtxAddr()->stFlowCtrl);
}

/*****************************************************************************
 函 数 名  : AT_GetUartRiCfgInfo
 功能描述  : 获取UART端口RING脚信号配置信息
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_UART_RI_CFG_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
AT_UART_RI_CFG_STRU* AT_GetUartRiCfgInfo(VOS_VOID)
{
    return &(AT_GetUartCtxAddr()->stRiConfig);
}

/*****************************************************************************
 函 数 名  : AT_GetUartRiStateInfo
 功能描述  : 获取UART端口RING脚信号状态信息
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_UART_RI_STATE_INFO_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : A00165503
    修改内容   : 新生成函数
*****************************************************************************/
AT_UART_RI_STATE_INFO_STRU* AT_GetUartRiStateInfo(VOS_VOID)
{
    return &(AT_GetUartCtxAddr()->stRiStateInfo);
}

/*****************************************************************************
 函 数 名  : AT_GetPortBuffCfgInfo
 功能描述  : 获取已经使用端口记录地址
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_COMM_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年09月21日
    作    者   : j00174725
    修改内容   : 新生成函数
*****************************************************************************/
AT_PORT_BUFF_CFG_STRU* AT_GetPortBuffCfgInfo(VOS_VOID)
{
    return &(AT_GetCommCtxAddr()->stPortBuffCfg);
}

/*****************************************************************************
 函 数 名  : AT_GetPortBuffCfg
 功能描述  : 获取短信缓存NV配置
 输入参数  : VOS_VOID
 输出参数  : VOS_VOID
 返 回 值  : AT_SMS_BUFF_CFG_ENUM_UINT8
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月28日
    作    者   : j00174725
    修改内容   : HSUART PHASE III
*****************************************************************************/
AT_PORT_BUFF_CFG_ENUM_UINT8  AT_GetPortBuffCfg(VOS_VOID)
{
    AT_COMM_CTX_STRU                   *pstCommCtx = VOS_NULL_PTR;

    pstCommCtx = AT_GetCommCtxAddr();

    return pstCommCtx->stPortBuffCfg.enSmsBuffCfg;
}

/*****************************************************************************
 函 数 名  : AT_InitPortBuffCfg
 功能描述  : 初始化记录使用clientID的表
 输入参数  : VOS_VOID
 输出参数  : VOS_VOID
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月28日
    作    者   : j00174725
    修改内容   : HSUART PHASE III
*****************************************************************************/
VOS_VOID AT_InitPortBuffCfg(VOS_VOID)
{
    AT_PORT_BUFF_CFG_STRU              *pstUsedClientIdTab = VOS_NULL_PTR;

    pstUsedClientIdTab = AT_GetPortBuffCfgInfo();

    TAF_MEM_SET_S(pstUsedClientIdTab, sizeof(AT_PORT_BUFF_CFG_STRU), 0xFF, sizeof(AT_PORT_BUFF_CFG_STRU));

    pstUsedClientIdTab->ucNum = 0;
}

/*****************************************************************************
 函 数 名  : AT_AddUsedClientId2Tab
 功能描述  : 将使用的clientID记录到全局变量中
 输入参数  : usClientId
 输出参数  : VOS_VOID
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月28日
    作    者   : j00174725
    修改内容   : HSUART PHASE III

  2.日    期   : 2014年2月14日
    作    者   : j00174725
    修改内容   : TQE
*****************************************************************************/
VOS_VOID AT_AddUsedClientId2Tab(VOS_UINT16 usClientId)
{
    AT_PORT_BUFF_CFG_STRU              *pstPortBuffCfg = VOS_NULL_PTR;
    VOS_UINT8                           ucIndex;

    if (usClientId >= AT_MAX_CLIENT_NUM)
    {
        return;
    }

    pstPortBuffCfg = AT_GetPortBuffCfgInfo();

    /*  排除内部通道 */
    if ( (usClientId >= AT_MIN_APP_CLIENT_ID)
      && (usClientId <= AT_MAX_APP_CLIENT_ID))
    {
        return;
    }


    if (pstPortBuffCfg->ucNum >= AT_MAX_CLIENT_NUM)
    {
        pstPortBuffCfg->ucNum = AT_MAX_CLIENT_NUM -1;
    }

    /* 循环查找是否已经记录过 */
    for (ucIndex = 0; ucIndex < pstPortBuffCfg->ucNum; ucIndex++)
    {
        if (usClientId == pstPortBuffCfg->ulUsedClientID[ucIndex])
        {
            return;
        }
    }

    /* 如果没有记录过，测记录到最后 */
    if (ucIndex == pstPortBuffCfg->ucNum)
    {
        pstPortBuffCfg->ulUsedClientID[ucIndex] = (VOS_UINT32)usClientId;
        pstPortBuffCfg->ucNum++;
    }
}

/*****************************************************************************
 函 数 名  : AT_RmUsedClientIdFromTab
 功能描述  : 将ClientID从全局变量中删除
 输入参数  : usClientId
 输出参数  : VOS_VOID
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月28日
    作    者   : j00174725
    修改内容   : HSUART PHASE III
*****************************************************************************/
VOS_VOID AT_RmUsedClientIdFromTab(VOS_UINT16 usClientId)
{
    AT_PORT_BUFF_CFG_STRU              *pstPortBuffCfg = VOS_NULL_PTR;
    VOS_UINT32                          ulIndex;

    pstPortBuffCfg = AT_GetPortBuffCfgInfo();

    if (pstPortBuffCfg->ucNum > AT_MAX_CLIENT_NUM)
    {
        pstPortBuffCfg->ucNum = AT_MAX_CLIENT_NUM;
    }

    /* 循环查找是否已经记录过 */
    for (ulIndex = 0; ulIndex < pstPortBuffCfg->ucNum; ulIndex++)
    {
        if (usClientId == pstPortBuffCfg->ulUsedClientID[ulIndex])
        {
            break;
        }
    }

    /* 如果没有记录过则直接退出 */
    if (ulIndex == pstPortBuffCfg->ucNum)
    {
        return;
    }

    /* 如果找到就删除对应的client */
    if (ulIndex == pstPortBuffCfg->ucNum - 1)
    {
        pstPortBuffCfg->ucNum--;
        pstPortBuffCfg->ulUsedClientID[ulIndex] = VOS_NULL_DWORD;

    }
    else
    {
        pstPortBuffCfg->ulUsedClientID[ulIndex] =
                    pstPortBuffCfg->ulUsedClientID[pstPortBuffCfg->ucNum - 1];
        pstPortBuffCfg->ulUsedClientID[pstPortBuffCfg->ucNum - 1] = VOS_NULL_DWORD;
        pstPortBuffCfg->ucNum--;
    }
}


/*****************************************************************************
 函 数 名  : AT_GetClientConfig
 功能描述  : 获取ClientConfig上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_CLIENT_CONFIGURATION_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月25日
    作    者   : f00179208
    修改内容   : 新生成函数

*****************************************************************************/
AT_CLIENT_CONFIGURATION_STRU* AT_GetClientConfig(
    AT_CLIENT_ID_ENUM_UINT16            enClientId
)
{
    return &(AT_GetClientCtxAddr(enClientId)->stClientConfiguration);
}

/*****************************************************************************
 函 数 名  : AT_GetClientCfgMapTbl
 功能描述  : 获取CLIENT CFG TAB上下文
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : AT_CLIENT_CFG_MAP_TAB_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月25日
    作    者   : f00179208
    修改内容   : 新生成函数

*****************************************************************************/
AT_CLIENT_CFG_MAP_TAB_STRU* AT_GetClientCfgMapTbl(VOS_UINT8 ucIndex)
{
    return &(g_astAtClientCfgMapTbl[ucIndex]);
}


/*****************************************************************************
 函 数 名  : AT_GetPrivacyFilterEnableFlg
 功能描述  : 获取NV过滤配置信息中是否需过滤的标识
 输入参数  : 无
 输出参数  : 无
 返 回 值  : VOS_UINT8 : 标识信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年07月01日
    作    者   : f00179208
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT8 AT_GetPrivacyFilterEnableFlg(VOS_VOID)
{
    return AT_GetModemPrivacyFilterCtxAddrFromModemId(MODEM_ID_0)->ucFilterEnableFlg;
}

/*****************************************************************************
 函 数 名  : AT_IsSupportReleaseRst
 功能描述  : 判断是否支持协议Rx版本
 输入参数  : enReleaseType   接入层版本号
 输出参数  : 无
 返 回 值  : VOS_UINT8
 调用函数  :
 被调函数  :

 修改历史     :
 1.日    期   : 2015年10月20日
   作    者   : w00316404
   修改内容   : 新生成函数

 2.日    期   : 2016年11月18日
   作    者   : wx270776
   修改内容   : DTS2016110401052
*****************************************************************************/
VOS_UINT8 AT_IsSupportReleaseRst(
    AT_ACCESS_STRATUM_REL_ENUM_UINT8    enReleaseType
)
{
    if (enReleaseType <= g_stReleaseInfo.enAccessStratumRel)
    {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}

/*****************************************************************************
 函 数 名  : AT_GetModemCLModeCtxAddrFromModemId
 功能描述  : 根据MODEM ID获取CL mode相关的上下文
 输入参数  : MODEM_ID_ENUM_UINT16                enModemId
 输出参数  : 无
 返 回 值  : AT_MODEM_SMS_CTX_STRU*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月27日
    作    者   : f00279542
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT8* AT_GetModemCLModeCtxAddrFromModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return &(g_astAtModemCtx[enModemId].stPlatformCapList.ucIsCLMode);
}

/*****************************************************************************
 函 数 名  : AT_GetCgpsCLockEnableFlgByModemId
 功能描述  : 根据MODEM ID获取AT^CGPSCLOCK的第一个参数
 输入参数  : MODEM_ID_ENUM_UINT16       enModemId
 输出参数  : 无
 返 回 值  : VOS_UINT8
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年7月11日
    作    者   : wx270776
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT8 AT_GetCgpsCLockEnableFlgByModemId(
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    return g_astAtModemCtx[enModemId].stAgpsCtx.ucAtCgpsClockEnableFlag;
}

/*****************************************************************************
 函 数 名  : AT_SetCgpsCLockEnableFlgByModemId
 功能描述  : 根据MODEM ID设置AT^CGPSCLOCK的第一个参数
 输入参数  : MODEM_ID_ENUM_UINT16       enModemId
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年7月11日
    作    者   : wx270776
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_SetCgpsCLockEnableFlgByModemId(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucEnableFLg
)
{
    g_astAtModemCtx[enModemId].stAgpsCtx.ucAtCgpsClockEnableFlag = ucEnableFLg;
}

