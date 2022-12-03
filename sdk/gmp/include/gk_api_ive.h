/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _GK_API_IVE_H_
#define _GK_API_IVE_H_

#include "ive.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_IVE_DMA(IVE_HANDLE *pIveHandle, IVE_DATA_S *pstSrc, IVE_DST_DATA_S *pstDst,IVE_DMA_CTRL_S *pstDmaCtrl, GK_BOOL bInstant);


GK_S32 GK_API_IVE_Filter(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_FILTER_CTRL_S *pstFltCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_CSC(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_CSC_CTRL_S *pstCscCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_FilterAndCSC(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_FILTER_AND_CSC_CTRL_S *pstFltCscCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Sobel(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDstH,IVE_DST_IMAGE_S *pstDstV, IVE_SOBEL_CTRL_S *pstSobelCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_MagAndAng(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDstMag,IVE_DST_IMAGE_S *pstDstAng, IVE_MAG_AND_ANG_CTRL_S *pstMagAndAngCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Dilate(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_DILATE_CTRL_S *pstDilateCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Erode(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_ERODE_CTRL_S *pstErodeCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Thresh(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_THRESH_CTRL_S *pstThrCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_And(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_IMAGE_S *pstDst, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Sub(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_IMAGE_S *pstDst, IVE_SUB_CTRL_S *pstSubCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Or(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_IMAGE_S *pstDst, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Integ(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_INTEG_CTRL_S *pstIntegCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Hist(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_MEM_INFO_S *pstDst,GK_BOOL bInstant);

GK_S32 GK_API_IVE_Thresh_S16(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_THRESH_S16_CTRL_S *pstThrS16Ctrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Thresh_U16(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_THRESH_U16_CTRL_S *pstThrU16Ctrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_16BitTo8Bit(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_16BIT_TO_8BIT_CTRL_S *pst16BitTo8BitCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_OrdStatFilter(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_ORD_STAT_FILTER_CTRL_S *pstOrdStatFltCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Map(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_MEM_INFO_S *pstMap,IVE_DST_IMAGE_S *pstDst, IVE_MAP_CTRL_S *pstMapCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_EqualizeHist(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_EQUALIZE_HIST_CTRL_S *pstEqualizeHistCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Add(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_IMAGE_S *pstDst, IVE_ADD_CTRL_S *pstAddCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Xor(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_IMAGE_S *pstDst, GK_BOOL bInstant);

GK_S32 GK_API_IVE_NCC(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_MEM_INFO_S *pstDst, GK_BOOL bInstant);

GK_S32 GK_API_IVE_CCL(IVE_HANDLE *pIveHandle, IVE_IMAGE_S *pstSrcDst, IVE_DST_MEM_INFO_S *pstBlob,IVE_CCL_CTRL_S *pstCclCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_GMM(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstFg,IVE_DST_IMAGE_S *pstBg, IVE_MEM_INFO_S *pstModel, IVE_GMM_CTRL_S *pstGmmCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_GMM2(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_IMAGE_S *pstFactor,IVE_DST_IMAGE_S *pstFg, IVE_DST_IMAGE_S *pstBg, IVE_DST_IMAGE_S *pstMatchModelInfo, IVE_MEM_INFO_S *pstModel,IVE_GMM2_CTRL_S *pstGmm2Ctrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_CannyHysEdge(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstEdge,IVE_DST_MEM_INFO_S *pstStack, IVE_CANNY_HYS_EDGE_CTRL_S *pstCannyHysEdgeCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_CannyEdge(IVE_IMAGE_S *pstEdge, IVE_MEM_INFO_S *pstStack);

GK_S32 GK_API_IVE_LBP(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,IVE_LBP_CTRL_S *pstLbpCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_NormGrad(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDstH,IVE_DST_IMAGE_S *pstDstV, IVE_DST_IMAGE_S *pstDstHV, IVE_NORM_GRAD_CTRL_S *pstNormGradCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_LKOpticalFlowPyr(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S astSrcPrevPyr[],IVE_SRC_IMAGE_S astSrcNextPyr[], IVE_SRC_MEM_INFO_S *pstPrevPts, IVE_MEM_INFO_S *pstNextPts,IVE_DST_MEM_INFO_S *pstStatus, IVE_DST_MEM_INFO_S *pstErr, IVE_LK_OPTICAL_FLOW_PYR_CTRL_S *pstLkOptiFlowPyrCtrl,GK_BOOL bInstant);

GK_S32 GK_API_IVE_STCandiCorner(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstCandiCorner,IVE_ST_CANDI_CORNER_CTRL_S *pstStCandiCornerCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_STCorner(IVE_SRC_IMAGE_S *pstCandiCorner, IVE_DST_MEM_INFO_S *pstCorner,IVE_ST_CORNER_CTRL_S *pstStCornerCtrl);

GK_S32 GK_API_IVE_GradFg(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstBgDiffFg, IVE_SRC_IMAGE_S *pstCurGrad,IVE_SRC_IMAGE_S *pstBgGrad, IVE_DST_IMAGE_S *pstGradFg, IVE_GRAD_FG_CTRL_S *pstGradFgCtrl, GK_BOOL bInstant);


GK_S32 GK_API_IVE_MatchBgModel(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstCurImg, IVE_DATA_S *pstBgModel,IVE_IMAGE_S *pstFgFlag, IVE_DST_IMAGE_S *pstBgDiffFg, IVE_DST_IMAGE_S *pstFrmDiffFg,IVE_DST_MEM_INFO_S *pstStatData, IVE_MATCH_BG_MODEL_CTRL_S *pstMatchBgModelCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_UpdateBgModel(IVE_HANDLE *pIveHandle, IVE_DATA_S *pstBgModel, IVE_IMAGE_S *pstFgFlag,IVE_DST_IMAGE_S *pstBgImg, IVE_DST_IMAGE_S *pstChgStaImg, IVE_DST_IMAGE_S *pstChgStaFg,IVE_DST_IMAGE_S *pstChgStaLife, IVE_DST_MEM_INFO_S *pstStatData, IVE_UPDATE_BG_MODEL_CTRL_S *pstUpdateBgModelCtrl,GK_BOOL bInstant);

GK_S32 GK_API_IVE_ANN_MLP_LoadModel(const GK_CHAR *pchFileName, IVE_ANN_MLP_MODEL_S *pstAnnMlpModel);

GK_VOID GK_API_IVE_ANN_MLP_UnloadModel(IVE_ANN_MLP_MODEL_S *pstAnnMlpModel);

GK_S32 GK_API_IVE_ANN_MLP_Predict(IVE_HANDLE *pIveHandle, IVE_SRC_DATA_S *pstSrc,IVE_LOOK_UP_TABLE_S *pstActivFuncTab, IVE_ANN_MLP_MODEL_S *pstAnnMlpModel, IVE_DST_DATA_S *pstDst,GK_BOOL bInstant);

GK_S32 GK_API_IVE_SVM_LoadModel(const GK_CHAR *pchFileName, IVE_SVM_MODEL_S *pstSvmModel);

GK_VOID GK_API_IVE_SVM_UnloadModel(IVE_SVM_MODEL_S *pstSvmModel);

GK_S32 GK_API_IVE_SVM_Predict(IVE_HANDLE *pIveHandle, IVE_SRC_DATA_S *pstSrc, IVE_LOOK_UP_TABLE_S *pstKernelTab,IVE_SVM_MODEL_S *pstSvmModel, IVE_DST_DATA_S *pstDstVote, GK_BOOL bInstant);

GK_S32 GK_API_IVE_SAD(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2,IVE_DST_IMAGE_S *pstSad, IVE_DST_IMAGE_S *pstThr, IVE_SAD_CTRL_S *pstSadCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Resize(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S astSrc[], IVE_DST_IMAGE_S astDst[],IVE_RESIZE_CTRL_S *pstResizeCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_CNN_LoadModel(const GK_CHAR *pchFileName, IVE_CNN_MODEL_S *pstCnnModel);

GK_VOID GK_API_IVE_CNN_UnloadModel(IVE_CNN_MODEL_S *pstCnnModel);

GK_S32 GK_API_IVE_CNN_Predict(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S astSrc[], IVE_CNN_MODEL_S *pstCnnModel,IVE_DST_DATA_S *pstDst, IVE_CNN_CTRL_S *pstCnnCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_CNN_GetResult(IVE_SRC_DATA_S *pstSrc, IVE_DST_MEM_INFO_S *pstDst,IVE_CNN_MODEL_S *pstCnnModel, IVE_CNN_CTRL_S *pstCnnCtrl);

GK_S32 GK_API_IVE_PerspTrans(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_RECT_U32_S astRoi[],IVE_SRC_MEM_INFO_S astPointPair[], IVE_DST_IMAGE_S astDst[], IVE_PERSP_TRANS_CTRL_S *pstPerspTransCtrl,GK_BOOL bInstant);

GK_S32 GK_API_IVE_KCF_GetMemSize(GK_U32 u32MaxObjNum, GK_U32 *pu32Size);

GK_S32 GK_API_IVE_KCF_CreateObjList(IVE_MEM_INFO_S *pstMem, GK_U32 u32MaxObjNum, IVE_KCF_OBJ_LIST_S *pstObjList);

GK_S32 GK_API_IVE_KCF_DestroyObjList(IVE_KCF_OBJ_LIST_S *pstObjList);

GK_S32 GK_API_IVE_KCF_CreateGaussPeak(GK_U3Q5 u3q5Padding, IVE_DST_MEM_INFO_S *pstGaussPeak);

GK_S32 GK_API_IVE_KCF_CreateCosWin(IVE_DST_MEM_INFO_S *pstCosWinX, IVE_DST_MEM_INFO_S *pstCosWinY);

GK_S32 GK_API_IVE_KCF_GetTrainObj(GK_U3Q5 u3q5Padding, IVE_ROI_INFO_S astRoiInfo[], GK_U32 u32ObjNum,IVE_MEM_INFO_S *pstCosWinX, IVE_MEM_INFO_S *pstCosWinY, IVE_MEM_INFO_S *pstGaussPeak,IVE_KCF_OBJ_LIST_S *pstObjList);

GK_S32 GK_API_IVE_KCF_Process(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_KCF_OBJ_LIST_S *pstObjList,IVE_KCF_PRO_CTRL_S *pstKcfProCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_KCF_GetObjBbox(IVE_KCF_OBJ_LIST_S *pstObjList, IVE_KCF_BBOX_S astBbox[],GK_U32 *pu32BboxObjNum, IVE_KCF_BBOX_CTRL_S *pstKcfBboxCtrl);

GK_S32 GK_API_IVE_KCF_JudgeObjBboxTrackState(IVE_ROI_INFO_S *pstRoiInfo, IVE_KCF_BBOX_S *pstBbox,GK_BOOL *pbTrackOk);

GK_S32 GK_API_IVE_KCF_ObjUpdate(IVE_KCF_OBJ_LIST_S *pstObjList, IVE_KCF_BBOX_S astBbox[],GK_U32 u32BboxObjNum);

GK_S32 GK_API_IVE_Hog(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_RECT_U32_S astRoi[],IVE_DST_BLOB_S astDst[], IVE_HOG_CTRL_S *pstHogCtrl, GK_BOOL bInstant);

GK_S32 GK_API_IVE_Query(IVE_HANDLE IveHandle, GK_BOOL *pbFinish, GK_BOOL bBlock);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
