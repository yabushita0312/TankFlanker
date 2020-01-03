﻿#include "define.h"
//
Myclass::Myclass() {
	std::string tempname;
	int i, j;
	WIN32_FIND_DATA win32fdt;
	HANDLE hFind;
	char mstr[64];								/*tank*/
	int mdata;							/*tank*/

	SetOutApplicationLogValidFlag(FALSE);					/*log*/
	mdata = FileRead_open("data/setting.txt", FALSE);
	FileRead_gets(mstr, 64, mdata); usegrab		= (bool)atof(getright(mstr).c_str());
	FileRead_gets(mstr, 64, mdata); ANTI		= (unsigned char)atof(getright(mstr).c_str());
	FileRead_gets(mstr, 64, mdata); YSync		= (bool)atof(getright(mstr).c_str());
	FileRead_gets(mstr, 64, mdata); if (YSync) { f_rate = 60.f; } else { f_rate = (float)atof(getright(mstr).c_str()); }
	FileRead_gets(mstr, 64, mdata); windowmode	= (bool)atof(getright(mstr).c_str());
	FileRead_gets(mstr, 64, mdata); drawdist	= (float)atof(getright(mstr).c_str());
	FileRead_gets(mstr, 64, mdata); gndx		= (int)atof(getright(mstr).c_str());
	FileRead_close(mdata);

	SetMainWindowText("Tank Flanker");
	//SetWindowStyleMode(4);
	SetWindowUserCloseEnableFlag(FALSE);
	SetAeroDisableFlag(TRUE);
	SetUsePixelLighting(TRUE);						/*ピクセルライティング*/
	if (ANTI >= 2) { SetCreateDrawValidGraphMultiSample(ANTI, 2); }		/*アンチエイリアス*/
	SetWaitVSyncFlag(YSync);						/*垂直同期*/
	ChangeWindowMode(windowmode);						/*窓表示*/
	SetUseDirect3DVersion(DX_DIRECT3D_11);					/*directX ver*/
	Set3DSoundOneMetre(1.0f);						/*3Dsound*/
	SetGraphMode(dispx, dispy, 32);						/*解像度*/
	DxLib_Init();								/*init*/
	Effekseer_Init(8000);							/*Effekseer*/
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);			/*Effekseer*/
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();			/*Effekseer*/
	//Effekseer_InitDistortion(1.0f);
	SetAlwaysRunFlag(TRUE);							/*background*/
	SetUseZBuffer3D(TRUE);							/*zbufuse*/
	SetWriteZBuffer3D(TRUE);						/*zbufwrite*/
	MV1SetLoadModelReMakeNormal(TRUE);					/*法線*/
	MV1SetLoadModelPhysicsWorldGravity(M_GR);				/*重力*/
	hFind = FindFirstFile("data/tanks/*", &win32fdt);
	if (hFind != INVALID_HANDLE_VALUE) {
		i = 0;
		do {
			if ((win32fdt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (win32fdt.cFileName[0] != '.')) {
				vecs.resize(i+1);
				vecs[i].name = win32fdt.cFileName;

				tempname = "data/tanks/" + vecs[i].name + "/data.txt";
				mdata = FileRead_open(tempname.c_str(), FALSE);
				FileRead_gets(mstr, 64, mdata); vecs[i].countryc = (int)(atof(getright(mstr).c_str()));
				for (j = 0; j < 4; ++j) {
					FileRead_gets(mstr, 64, mdata); vecs[i].spdflont[3 - j] = (float)atof(getright(mstr).c_str()) / 3.6f;
				}
				for (j = 0; j < 4; ++j) {
					FileRead_gets(mstr, 64, mdata); vecs[i].spdback[j] = (float)atof(getright(mstr).c_str()) / 3.6f;
				}
				FileRead_gets(mstr, 64, mdata); vecs[i].vehicle_RD = deg2rad((float)atof(getright(mstr).c_str()));
				for (j = 0; j < 4; ++j) {
					FileRead_gets(mstr, 64, mdata); vecs[i].armer[j] = (float)atof(getright(mstr).c_str());
				}
				FileRead_gets(mstr, 64, mdata); vecs[i].gun_lim_LR = (bool)atof(getright(mstr).c_str());
				for (j = 0; j < 4; ++j) {
					FileRead_gets(mstr, 64, mdata); vecs[i].gun_lim_[j] = deg2rad((float)atof(getright(mstr).c_str()));
				}
				FileRead_gets(mstr, 64, mdata); vecs[i].gun_RD = deg2rad((float)atof(getright(mstr).c_str())) / f_rate;
				FileRead_gets(mstr, 64, mdata); vecs[i].reloadtime[0] = (int)(atof(getright(mstr).c_str())*f_rate);
				vecs[i].reloadtime[1] = 10;
				FileRead_gets(mstr, 64, mdata); vecs[i].ammosize = (float)atof(getright(mstr).c_str()) / 1000.f;
				for (j = 0; j < 3; ++j) {
					FileRead_gets(mstr, 64, mdata); vecs[i].ammotype[j] = (int)atof(getright(mstr).c_str());
					FileRead_gets(mstr, 64, mdata); vecs[i].gun_speed[j] = (float)atof(getright(mstr).c_str());
					FileRead_gets(mstr, 64, mdata); vecs[i].pene[j] = (float)atof(getright(mstr).c_str());
				}
				FileRead_close(mdata);
				i++;
			}
		} while (FindNextFile(hFind, &win32fdt));
	}//else{ return false; }
	FindClose(hFind);
	SetUseASyncLoadFlag(TRUE);
		for (j = 0; j < vecs.size(); ++j) {
			tempname = "data/tanks/" + vecs[j].name + "/model.mv1";		vecs[j].model = MV1LoadModel(tempname.c_str());
			tempname = "data/tanks/" + vecs[j].name + "/col.mv1";		vecs[j].colmodel = MV1LoadModel(tempname.c_str());
			tempname = "data/tanks/" + vecs[j].name + "/in/model.mv1";	vecs[j].inmodel = MV1LoadModel(tempname.c_str());
		}
		for (j = 0; j < 13; ++j) {
			if (j < 1) { tempname = "data/audio/se/engine/shift.wav"; }
			else if (j < 8) { tempname = "data/audio/se/eject/" + std::to_string(j - 1) + ".wav"; }
			else if (j < 13) { tempname = "data/audio/se/load/" + std::to_string(j - 8) + ".wav"; }
			se_[j] = LoadSoundMem(tempname.c_str());
		}
		for (j = 0; j < 4; ++j) {
			tempname = "data/ui/ammo_" + std::to_string(j) + ".bmp";	ui_reload[j] = LoadGraph(tempname.c_str());
		}/*弾0,弾1,弾2,空弾*/
	SetUseASyncLoadFlag(FALSE);
	//return true;
}
void Myclass::write_option(void) {
}
bool Myclass::set_fonts(int arg_num, ...) {
	va_list args;
	int value, i;
	if (arg_num < 1) { return false; }
	va_start(args, arg_num);
	SetUseASyncLoadFlag(TRUE);
		for (i = 0; i < arg_num; i++) {
			value = va_arg(args, int);
			fonts.push_back(CreateFontToHandle(NULL, x_r(value), y_r(value / 3), DX_FONTTYPE_ANTIALIASING_EDGE));
		}
	SetUseASyncLoadFlag(FALSE);
	va_end(args);
	return true;
}
bool Myclass::set_veh(void) {
	std::string tempname;
	int i, j, k;
	LONGLONG waits;								/*時間取得*/
	for (j = 0; j < vecs.size(); ++j) {
		vecs[j].meshes = MV1GetMeshNum(vecs[j].model);
		vecs[j].frames = MV1GetFrameNum(vecs[j].model);
		vecs[j].colmeshes = MV1GetMeshNum(vecs[j].colmodel);
		for (i = 0; i < vecs[j].frames; ++i) { vecs[j].loc.push_back(MV1GetFramePosition(vecs[j].model, i)); }
		for (i = 0; i < 4; ++i) { vecs[j].coloc[i] = MV1GetFramePosition(vecs[j].colmodel, 5 + i); }
	}
	//エフェクト------------------------------------------------------------//
	k = 0;
	for (j = 0; j < effects; ++j) {
		i = 0;
		while (ProcessMessage() == 0) {
			tempname = "data/effect/" + std::to_string(j) + ".efk";
			effHndle[j] = LoadEffekseerEffect(tempname.c_str());
			i++;

			waits = GetNowHiPerformanceCount();
			SetDrawScreen(DX_SCREEN_BACK);
			DrawFormatString(0, (18 * k), GetColor(0, 255, 0), "エフェクト読み込み中…%d/%d", j, effects);//
			if (effHndle[j] != -1) {
				k++; DrawFormatString(0, (18 * k), GetColor(255, 255, 0), "エフェクト読み込み成功…%d", j);
			}
			if (i > f_rate) {
				k++; DrawFormatString(0, (18 * k), GetColor(255, 0, 0), "エフェクト読み込み失敗…%d", j);
			}
			Screen_Flip(waits);
			if (effHndle[j] != -1) { break; }
			if (i > f_rate) { break; }
		}
		k++;
	}
	i = 0;
	while (ProcessMessage() == 0) {
		waits = GetNowHiPerformanceCount();
		Screen_Flip(waits);
		i++;
		if (i > f_rate) { break; }
	}
	return true;
}
int Myclass::window_choosev(void) {
	SetMousePoint(x_r(960),y_r(969));
	SetMouseDispFlag(TRUE);
	int font18 = CreateFontToHandle(NULL, x_r(18), y_r(18 / 3), DX_FONTTYPE_ANTIALIASING);
	int font72 = CreateFontToHandle(NULL, x_r(72), y_r(72 / 3), DX_FONTTYPE_ANTIALIASING);
	int i = 0, k, l = 0, m, x = 0, y = 0;
	int all = (int)vecs.size();
	int mousex, mousey;
	float real = 0.f, r = 5.f, unt = 0.f;
	LONGLONG waits;
	while (ProcessMessage() == 0) {
		waits = GetNowHiPerformanceCount();
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { i = -1; break; }				//end
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
			differential(real, deg2rad(360 * l / all), 0.05f);
			setcv(1.0f, 100.0f, VGet(-sin(real)*(10.f + r), 1, -cos(real)*(10.f + r)), VGet(-sin(real)*r, 2, -cos(real)*r), VGet(0, 1.0f, 0), 45.0f);
			SetLightDirection(VSub(VGet(-sin(real)*r, 2, -cos(real)*r), VGet(-sin(real)*(10.f + r), 4, -cos(real)*(10.f + r))));
			for (k = 0; k < all; k++) {
				MV1SetPosition(vecs[k].model, VGet(-sin(deg2rad(360 * k / all))*r, 0, -cos(deg2rad(360 * k / all))*r));
				MV1SetRotationXYZ(vecs[k].model, VGet(0, deg2rad((360 * k / all + 30)), 0));
				MV1DrawModel(vecs[k].model);
			}
			DrawFormatStringToHandle(x_r(960) - GetDrawFormatStringWidthToHandle(font72, "%s", vecs[i].name.c_str()) / 2, y_r(154), GetColor(0, 255, 0), font72, "%s", vecs[i].name.c_str());
			if ((GetRand(99) + 1) > (int)abs(real - deg2rad(360 * l / all)) / DX_PI_F * 100) { DrawFormatStringToHandle(x_r(850), y_r(850), GetColor(0, 255, 0), font18, "MAX SPD    : %5.2f km/h", vecs[i].spdflont[3] * 3.6f); }
			if ((GetRand(99) + 1) > (int)abs(real - deg2rad(360 * l / all)) / DX_PI_F * 100) { DrawFormatStringToHandle(x_r(850), y_r(870), GetColor(0, 255, 0), font18, "BACK SPD   : %5.2f km/h", vecs[i].spdback[3] * 3.6f); }
			if ((GetRand(99) + 1) > (int)abs(real - deg2rad(360 * l / all)) / DX_PI_F * 100) { DrawFormatStringToHandle(x_r(1140), y_r(810), GetColor(0, 255, 0), font18, "TURN SPEED : %5.2f deg/s", vecs[i].vehicle_RD * 180 / DX_PI_F); }
			if ((GetRand(99) + 1) > (int)abs(real - deg2rad(360 * l / all)) / DX_PI_F * 100) { DrawFormatStringToHandle(x_r(1120), y_r(580), GetColor(0, 255, 0), font18, "MAX ARMER  : %5.2f mm", vecs[i].armer[0]); }
			if ((GetRand(99) + 1) > (int)abs(real - deg2rad(360 * l / all)) / DX_PI_F * 100) { DrawFormatStringToHandle(x_r(650), y_r(410), GetColor(0, 255, 0), font18, "GUN RAD     : %5.2f°～%5.2f°", vecs[i].gun_lim_[2] * 180 / DX_PI_F, vecs[i].gun_lim_[3] * 180 / DX_PI_F); }
			if ((GetRand(99) + 1) > (int)abs(real - deg2rad(360 * l / all)) / DX_PI_F * 100) { DrawFormatStringToHandle(x_r(650), y_r(430), GetColor(0, 255, 0), font18, "GUN CALIBER : %05.1fmm", vecs[i].ammosize*1000.f); }
			//
			DrawFormatStringToHandle(x_r(0), y_r(18 * 1), GetColor(0, 255, 0), font18, "%s", "SETTING");
			DrawFormatStringToHandle(x_r(0), y_r(18 * 2), GetColor(0, 255, 0), font18, " 人の物理演算         : %s", usegrab ? "TRUE" : "FALSE");
			DrawFormatStringToHandle(x_r(0), y_r(18 * 3), GetColor(0, 255, 0), font18, " アンチエイリアス倍率 : x%d", ANTI);
			DrawFormatStringToHandle(x_r(0), y_r(18 * 4), GetColor(0, 255, 0), font18, " 垂直同期             : %s", YSync ? "TRUE" : "FALSE");
			DrawFormatStringToHandle(x_r(0), y_r(18 * 5), GetColor(0, 255, 0), font18, " ウィンドウor全画面   : %s", windowmode ? "TRUE" : "FALSE");
			DrawFormatStringToHandle(x_r(0), y_r(18 * 6), GetColor(0, 255, 0), font18, " 木の描画距離         : %5.2f m", drawdist);
			DrawFormatStringToHandle(x_r(0), y_r(18 * 7), GetColor(0, 255, 0), font18, " 地面のクォリティ     : x%d", gndx);
			//
			GetMousePoint(&mousex, &mousey);
			if (inm(x_r(360), y_r(340), x_r(400), y_r(740))) {
				m = GetColor(255, 255, 0);
				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { m = GetColor(255, 0, 0); ++x; if (x == 1) { l++; i++; if (i > all - 1) { i = 0; } } } else { x = 0; }
			}
			else { m = GetColor(0, 255, 0); }
			DrawBox(x_r(360), y_r(340), x_r(400), y_r(740), m, FALSE);
			DrawFormatStringToHandle(x_r(396) - GetDrawFormatStringWidthToHandle(font18, "%s", "<"), y_r(531), GetColor(255, 255, 255), font18, "%s", "<");
			//
			if (inm(x_r(1520), y_r(340), x_r(1560), y_r(740))) {
				m = GetColor(255, 255, 0);
				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { m = GetColor(255, 0, 0); ++y; if (y == 1) { l--;  i--; if (i < 0) { i = all - 1; } } } else { y = 0; }
			}
			else { m = GetColor(0, 255, 0); }
			DrawBox(x_r(1520), y_r(340), x_r(1560), y_r(740), m, FALSE);
			DrawFormatStringToHandle(x_r(1524), y_r(531), GetColor(255, 255, 255), font18, "%s", ">");
			//
			if (inm(x_r(760), y_r(960), x_r(1160), y_r(996))) {
				m = GetColor(255, 200, 0);
				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { break; }
			}
			else { m = GetColor(200, 100, 0); }
			DrawBox(x_r(760), y_r(960), x_r(1160), y_r(996), m, FALSE);
			DrawFormatStringToHandle(x_r(960) - GetDrawFormatStringWidthToHandle(font18, "%s", "戦闘開始") / 2, y_r(969), GetColor(255, 255, 255), font18, "%s", "戦闘開始");
		Screen_Flip(waits);
	}
	DeleteFontToHandle(font18);
	DeleteFontToHandle(font72);
	if (i != -1) {
		while (ProcessMessage() == 0 && unt <= 0.9f) {
			waits = GetNowHiPerformanceCount();
			SetDrawScreen(DX_SCREEN_BACK);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255.f*unt));
			DrawBox(0, 0, dispx, dispy, GetColor(0, 0, 0), TRUE);
			differential(unt, 1.f, 0.05f);
			Screen_Flip(waits);
		}
	}
	return i;
}
void Myclass::set_viewrad(VECTOR vv) {
	view = vv;
	view_r = vv;
}
void Myclass::set_view_r(void) {
	int px, py;
	GetMousePoint(&px, &py);
	view.z += (float)GetMouseWheelRotVol() / 10.0f; 
	if (view.z < 0.1f) { view.z = 0.1f; view_r.z = view.z; }
	if (view.z > 2.f) { view.z = 2.f; }
	//x_r(960), y_r(540)
	view.y += (float)(px - dispx / 2) / dispx * dispx / 640 * 1.0f;
	view.x += (float)(py - dispy / 2) / dispy * dispy / 480 * 1.0f;
	if (view.x > deg2rad(35)) { view.x = deg2rad(35); }
	if (view.x < deg2rad(-35)) { view.x = deg2rad(-35); }
	view_r = VAdd(view_r, VScale(VSub(view, view_r), 0.1f));
	SetMousePoint(x_r(960), y_r(540));
}
void Myclass::Screen_Flip(LONGLONG waits){
	ScreenFlip();
	if (!YSync) { do {} while (GetNowHiPerformanceCount() - waits < 1000000.0f / f_rate); }
}
Myclass::~Myclass() {
	Effkseer_End();
	DxLib_End();
}
void Myclass::set_se_vol(unsigned char size) {
	for (int i = 0; i < 13; ++i) { ChangeVolumeSoundMem(size, se_[i]); }
}
void Myclass::play_sound(int p1) {
	PlaySoundMem(se_[p1], DX_PLAYTYPE_BACK, TRUE);
}
//
HUMANS::HUMANS(bool useg,float frates){
	usegrab = useg;
	f_rate = frates;
}
void HUMANS::set_humans(int inmod) {
	std::string tempname = "結月ゆかり",tempname2;
	int i, j;
	//load
	SetUseASyncLoadFlag(FALSE);
	inmodel_handle = MV1DuplicateModel(inmod);
	inflames = MV1GetFrameNum(inmodel_handle);
	if (inflames - bone_in_turret >= 1) { human = inflames - bone_in_turret; } else { human = 1; }/*乗員*/
	pos_old.resize(inflames);
	hum.resize(human);
	for (i = 0; i < human; ++i) {
		if (usegrab) { MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME); }
		else { MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC); }

		tempname2 = "data/chara/" + tempname + "/model.mv1";
		hum[i].obj = MV1LoadModel(tempname2.c_str());
	}
	//
	for (i = 0; i < human; ++i) {
		for (j = 0; j < MV1GetMaterialNum(hum[i].obj); ++j) {
			MV1SetMaterialDifColor(hum[i].obj, j, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
			MV1SetMaterialSpcColor(hum[i].obj, j, GetColorF(0.35f, 0.32f, 0.28f, 0.5f));
			MV1SetMaterialSpcPower(hum[i].obj, j, 1.0f);
		}
		for (j = 0; j < animes; ++j) {
			hum[i].amine[j] = MV1AttachAnim(hum[i].obj, j, -1, TRUE);
			hum[i].time[j] = 0.0f;
			hum[i].alltime[j] = MV1GetAttachAnimTotalTime(hum[i].obj, hum[i].amine[j]);
			hum[i].per[j] = 0.f;
		}
	}
	for (i = 0; i < voice; ++i) {
		hum[0].voices[i] = MV1AttachAnim(hum[0].obj, animes + i, -1, TRUE);
		hum[0].voicealltime[i] = MV1GetAttachAnimTotalTime(hum[0].obj, hum[0].voices[i]);
		tempname2 = "data/chara/" + tempname + "/" + std::to_string(i) + ".wav";
		hum[0].vsound[i] = LoadSoundMem(tempname2.c_str());
	}
	//
	MV1SetMatrix(inmodel_handle, MGetTranslate(VGet(0, 0, 0)));
	for (i = 0; i < inflames; i++) {
		locin.push_back(MV1GetFramePosition(inmodel_handle, i));
		MV1ResetFrameUserLocalMatrix(inmodel_handle, i);
	}
	hum[0].neck = 0;
	do { tempname = MV1GetFrameName(hum[0].obj, hum[0].neck++); } while (tempname != "首");
	if (hum[0].neck == 0) { hum[0].neck = 121; }//暫定処置
	MV1SetMatrix(hum[0].obj, MGetTranslate(VGet(0, 0, 0)));
	hum[0].nvec = VSub(MV1GetFramePosition(hum[0].obj, hum[0].neck), MV1GetFramePosition(hum[0].obj, hum[0].neck - 1));
	first = false;
	hum[0].vflug = -1;
	return;
}
void HUMANS::set_humanvc_vol(unsigned char size) {
	for (int i = 0; i < voice; ++i) { ChangeVolumeSoundMem(size, hum[0].vsound[i]); }
}
void HUMANS::set_humanmove(players player, VECTOR rad, float fps) {
	float tmpft, tmpfy;
	int i, j,inflm=inflames;
	bool grab = usegrab;
	if (!first) { MV1SetMatrix(inmodel_handle, player.ps_m); }
	for (i = 0; i < inflm; ++i) { pos_old[i] = MV1GetFramePosition(inmodel_handle, i); }
	MV1SetMatrix(inmodel_handle, player.ps_m);
	for (i = 0; i < inflm; ++i) {
		if (i == bone_trt) {
			MV1SetFrameUserLocalMatrix(inmodel_handle, i, player.ps_t);
		}
		else if (i == bone_gun1) {
			MV1SetFrameUserLocalMatrix(inmodel_handle, i, MMult(MMult(MGetRotX(player.gunrad.y), MGetTranslate(VSub(locin[i], locin[bone_trt]))), player.ps_t));
		}
		else if (i == bone_gun2) {
			MV1SetFrameUserLocalMatrix(inmodel_handle, i, MGetTranslate(VAdd(VSub(player.ptr->loc[i], player.ptr->loc[bone_gun1]), VGet(0, 0, player.fired))));
		}
		else if (i >= bone_hatch &&( i != 9 || i != 10 )) {
			MV1SetFrameUserLocalMatrix(inmodel_handle, i, MMult(MMult(MGetRotY(player.gunrad.x), MGetTranslate(VSub(locin[i], locin[bone_trt]))), player.ps_t));
		}
	}

	if (rad.z >= 0.1f) { in_f = false; }
	if (rad.z == 0.1f && !in_f) {
		for (j = 1; j < human; ++j) { MV1PhysicsResetState(hum[j].obj); }
		in_f = true;
	}
	for (i = 0; i < human; ++i) {
		if (i == 0) {
			hum[i].per[0] = 0.f;
			if (player.speed >= 30.f / 3.6f) { hum[i].per[1] += (1.0f - hum[i].per[1]) * 0.1f; } else { hum[i].per[1] *= 0.9f; }			/*座る*/
			hum[i].per[2] = 1.f;
			if (hum[i].vflug == -1) { hum[i].per[3] *= 0.9f; } else { hum[i].per[3] += (1.0f - hum[i].per[3]) * 0.1f; } hum[i].per[0] = 1.0f - hum[i].per[3];			/*無線*/
		}
		else {
			hum[i].per[0] = 1.f;
			hum[i].per[1] = 1.f;
			hum[i].per[2] = 1.f;
			hum[i].per[3] = 0.f;
		}
	}
	//反映
	for (int k = 0; k < divi; ++k) {
		for (i = 0; i < human; ++i) {
			MV1SetMatrix(hum[i].obj, MMult(MMult(MGetRotY(player.yrad + player.gunrad.x), MGetRotVec2(VGet(0, 1.f, 0), player.nor)), MGetTranslate(VAdd(pos_old[bone_in_turret + i], VScale(VSub(MV1GetFramePosition(inmodel_handle, bone_in_turret + i), pos_old[bone_in_turret + i]), (float)(1 + k) / divi)))));//MMult(MGetRotX(player.xnor), MGetRotZ(player.znor))
			if (i == 0) {
				/*首振り*/
				tmpft = atanf(sin(rad.y - player.yrad - player.gunrad.x));
				if (tmpft > deg2rad(40)) { tmpft = deg2rad(40); }
				if (tmpft < deg2rad(-40)) { tmpft = deg2rad(-40); }
				tmpfy = -rad.x;
				if (tmpfy > deg2rad(20)) { tmpfy = deg2rad(20); }
				if (tmpfy < deg2rad(-20)) { tmpfy = deg2rad(-20); }
				MV1SetFrameUserLocalMatrix(hum[i].obj, hum[i].neck, MMult(MGetTranslate(hum[i].nvec), MMult(MGetRotX(tmpfy), MGetRotY(tmpft))));
				//voice
				if (hum[i].vflug != -1) {
					if (hum[i].voicetime < hum[i].voicealltime[hum[i].vflug]) {
						if (hum[i].voicetime == 0.0f) { PlaySoundMem(hum[i].vsound[hum[i].vflug], DX_PLAYTYPE_BACK, TRUE); }
						MV1SetAttachAnimTime(hum[i].obj, hum[i].voices[hum[i].vflug], hum[i].voicetime); hum[i].voicetime += 60.0f / divi / f_rate;//
					}
					else {
						hum[i].vflug = -1;
						hum[i].voicetime = 0.0f;
					}
				}
			}
			for (j = 0; j < animes; ++j) {
				MV1SetAttachAnimBlendRate(hum[i].obj, hum[i].amine[j], hum[i].per[j]);
				MV1SetAttachAnimTime(hum[i].obj, hum[i].amine[j], hum[i].time[j]); hum[i].time[j] += 30.0f / divi / f_rate;//
				if (j != 2) { if (hum[i].time[j] >= hum[i].alltime[j]) { hum[i].time[j] = 0.0f; } }
			}
			//
			if (grab) { if (!first) { MV1PhysicsResetState(hum[i].obj); } else { MV1PhysicsCalculation(hum[i].obj, 1000.0f / divi / f_rate); } }
			//
			if (i == 0) { if (!in_f) { break; } }
		}
	}
	first = true;
}
void HUMANS::draw_human(int p1) {
	MV1DrawModel(hum[p1].obj);
}
void HUMANS::draw_humanall() {
	int i;
	MV1DrawModel(inmodel_handle);
	for (i = 0; i < human; ++i) { draw_human(i); }
}
void HUMANS::delete_human(void) {
	int i;
	MV1DeleteModel(inmodel_handle);
	for (i = 0; i < human; ++i) { MV1DeleteModel(hum[i].obj); }
	for (i = 0; i < voice; ++i) { DeleteSoundMem(hum[0].vsound[i]); }
	hum.clear();
	locin.clear();
	pos_old.clear();
}
void HUMANS::start_humanvoice(int p1) {
	hum[0].vflug = p1;
}
void HUMANS::start_humananime(int p1) {
	for (int i = 0; i < human; ++i) { hum[i].time[p1] = 0.0f; }
}
//
MAPS::MAPS(int map_size, float draw_dist){
	groundx = map_size*1024;
	drawdist = draw_dist;
	//shadow----------------------------------------------------------------//
	shadow_near = MakeShadowMap(8192, 8192);				/*近影*/
	SetShadowMapAdjustDepth(shadow_near, 0.0005f);
	shadow_seminear = MakeShadowMap(8192, 8192);				/*近影*/
	shadow_far = MakeShadowMap(8192, 8192);					/*マップ用*/
	//map-------------------------------------------------------------------//
	SetUseASyncLoadFlag(TRUE);
	sky_sun = LoadGraph("data/sun.png");					/*太陽*/
	texo = LoadGraph("data/nm.png");					/*轍*/
	texp = MakeScreen(groundx, groundx, FALSE);				/*ノーマルマップ*/
	texn = MakeScreen(groundx, groundx, FALSE);				/*実マップ*/
	SetUseASyncLoadFlag(FALSE);
}
void MAPS::set_map_readyb(int set){
	std::string tempname,tempname2;
	lightvec = VGet(0.5f, -0.5f, 0.5f);
	SetUseASyncLoadFlag(TRUE);
		if (set == 0) { tempname = "map"; }
		tempname2 = "data/" + tempname + "/tree/model.mv1";		tree.mnear = MV1LoadModel(tempname2.c_str());			/*近木*/
		tempname2 = "data/" + tempname + "/tree/model2.mv1";		tree.mfar = MV1LoadModel(tempname2.c_str());			/*遠木*/
		tempname2 = "data/" + tempname + "/SandDesert_04_00344_FWD.png";texl = LoadGraph(tempname2.c_str());				/*nor*/
		tempname2 = "data/" + tempname + "/SandDesert_04_00344_NM.png";	texm = LoadGraph(tempname2.c_str());				/*nor*/
		tempname2 = "data/" + tempname + "/map.mv1";			m_model = MV1LoadModel(tempname2.c_str());			/*map*/
		tempname2 = "data/" + tempname + "/sky/model_sky.mv1";		sky_model = MV1LoadModel(tempname2.c_str());			/*sky*/
		tempname2 = "data/" + tempname + "/grass/grass.png";		graph = LoadGraph(tempname2.c_str());				/*grass*/
		tempname2 = "data/" + tempname + "/grass/grass.mqo";		grass = MV1LoadModel(tempname2.c_str());			/*grass*/
		tempname2 = "data/" + tempname + "/grass/gg.png";		GgHandle = LoadGraph(tempname2.c_str());			/*地面草*/
	SetUseASyncLoadFlag(FALSE);
	return;
}
bool MAPS::set_map_ready() {
	MV1_COLL_RESULT_POLY HitPoly;
	VECTOR tmpvect;
	char x, y, rate;
	int i, j;

	tree.treesort.resize(treec);
	tree.pos.resize(treec);
	tree.rad.resize(treec);
	MV1SetScale(sky_model, VGet(0.2f, 0.2f, 0.2f));

	SetUseASyncLoadFlag(TRUE);
		for (j = 0; j < treec; ++j) {
			tree.nears.push_back(MV1DuplicateModel(tree.mnear));
			tree.fars.push_back(MV1DuplicateModel(tree.mfar));
			tree.hit.push_back(true);//
		}
	SetUseASyncLoadFlag(FALSE);

	MV1SetupCollInfo(m_model, 0, map_x / 5, map_x / 5, map_y / 5);
	SetFogStartEnd(10.0f, 1400.0f);					/*fog*/
	SetFogColor(150, 150, 175);					/*fog*/
	SetLightDirection(lightvec);
	SetShadowMapLightDirection(shadow_near, lightvec);
	SetShadowMapLightDirection(shadow_seminear, lightvec);
	SetShadowMapLightDirection(shadow_far, lightvec);
	SetShadowMapDrawArea(shadow_far, VGet(-(float)map_x / 2.f, -(float)map_x / 2.f, -(float)map_y / 2.f), VGet((float)map_x / 2.f, (float)map_x / 2.f, (float)map_y / 2.f));

	rate = 96;

	SetDrawScreen(texp);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		for (x = 0; x < rate; x++) { for (y = 0; y < rate; y++) { DrawExtendGraph(groundx * x / rate, groundx * y / rate, groundx* (x + 1) / rate, groundx * (y + 1) / rate, texl, FALSE); } }
	MV1SetTextureGraphHandle(m_model, 0, texp, FALSE);
	SetDrawScreen(texn);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(0, 0, groundx, groundx, GetColor(121, 121, 255), TRUE);
		for (x = 0; x < 32; x++) { for (y = 0; y < 32; y++) { DrawExtendGraph(groundx * x / rate, groundx * y / rate, groundx* (x + 1) / rate, groundx * (y + 1) / rate, texm, TRUE); } }
	MV1SetTextureGraphHandle(m_model, 1, texn, FALSE);
		/*grass*/
		vnum = 0;
		pnum = 0;
		MV1SetupReferenceMesh(grass, -1, TRUE);				/*参照用メッシュの作成*/
		RefMesh = MV1GetReferenceMesh(grass, -1, TRUE);			/*参照用メッシュの取得*/
		IndexNum = RefMesh.PolygonNum * 3 * grasss;			/*インデックスの数を取得*/
		VerNum = RefMesh.VertexNum * grasss;				/*頂点の数を取得*/

		grassver = new VERTEX3D[VerNum];
		grassind = new DWORD[IndexNum];
		//grassver.resize(VerNum);					/*頂点データとインデックスデータを格納するメモリ領域の確保*/
		//grassind.resize(IndexNum);					/*頂点データとインデックスデータを格納するメモリ領域の確保*/

		for (i = 0; i < grasss; ++i) {
			tmpvect = VGet((float)(-5000 + GetRand(10000)) / 10.0f, 0.0f, (float)(-5000 + GetRand(10000)) / 10.0f);
			//
			SetDrawScreen(texp);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
			DrawRotaGraph( (int)(groundx * (0.5f + tmpvect.x / (float)map_x)), (int)(groundx * (0.5f - tmpvect.z / (float)map_y)), 8.f*groundx / 1024 / 128.0f, 0, GgHandle, TRUE );

			HitPoly = MV1CollCheck_Line(m_model, 0, VAdd(tmpvect, VGet(0.0f, (float)map_x, 0.0f)), VAdd(tmpvect, VGet(0.0f, -(float)map_x, 0.0f)));
			if (HitPoly.HitFlag) {
				MV1SetMatrix(
					grass,
					MMult(
						MGetScale(VGet((float)(200 + GetRand(400)) / 100.0f, (float)(25 + GetRand(100)) / 100.0f, (float)(200 + GetRand(400)) / 100.0f)),
						MMult(
							MMult( MGetRotY(deg2rad(GetRand(360))), MGetRotVec2(VGet(0, 1.f, 0), HitPoly.Normal) ),
							MGetTranslate(HitPoly.HitPosition)
						)
					)
				);
			}
			//MV1SetScale(grass, VGet((float)(200 + GetRand(400)) / 100.0f, (float)(25 + GetRand(100)) / 100.0f, (float)(200 + GetRand(400)) / 100.0f));	/*拡大*/
			MV1RefreshReferenceMesh(grass, -1, TRUE);		/*参照用メッシュの更新*/
			RefMesh = MV1GetReferenceMesh(grass, -1, TRUE);		/*参照用メッシュの取得*/
			for (j = 0; j < RefMesh.VertexNum; ++j) {
				grassver[j + vnum].pos = RefMesh.Vertexs[j].Position;
				grassver[j + vnum].norm = RefMesh.Vertexs[j].Normal;
				grassver[j + vnum].dif = RefMesh.Vertexs[j].DiffuseColor;
				grassver[j + vnum].spc = RefMesh.Vertexs[j].SpecularColor;
				grassver[j + vnum].u = RefMesh.Vertexs[j].TexCoord[0].u;
				grassver[j + vnum].v = RefMesh.Vertexs[j].TexCoord[0].v;
				grassver[j + vnum].su = RefMesh.Vertexs[j].TexCoord[1].u;
				grassver[j + vnum].sv = RefMesh.Vertexs[j].TexCoord[1].v;
			}
			for (j = 0; j < RefMesh.PolygonNum; ++j) {
				grassind[j * 3 + 0 + pnum] = (WORD)RefMesh.Polygons[j].VIndex[0] + vnum;
				grassind[j * 3 + 1 + pnum] = (WORD)RefMesh.Polygons[j].VIndex[1] + vnum;
				grassind[j * 3 + 2 + pnum] = (WORD)RefMesh.Polygons[j].VIndex[2] + vnum;
			}
			vnum += RefMesh.VertexNum;
			pnum += RefMesh.PolygonNum * 3;
		}
		VerBuf = CreateVertexBuffer(VerNum, DX_VERTEX_TYPE_NORMAL_3D);
		IndexBuf = CreateIndexBuffer(IndexNum, DX_INDEX_TYPE_32BIT);
		SetVertexBufferData(0, grassver, VerNum, VerBuf);
		SetIndexBufferData(0, grassind, IndexNum, IndexBuf);
		MV1SetTextureGraphHandle(m_model, 0, texp, FALSE);
		/*tree,shadow*/
	ShadowMap_DrawSetup(shadow_far);
		for (i = 0; i < treec; ++i) {
			tmpvect = VGet((float)(-5000 + GetRand(10000)) / 10.0f, 0.0f, (float)(-5000 + GetRand(10000)) / 10.0f);
			HitPoly = MV1CollCheck_Line(m_model, 0, VAdd(tmpvect, VGet(0.0f, 100.0f, 0.0f)), VAdd(tmpvect, VGet(0.0f, -100.0f, 0.0f)));
			if (HitPoly.HitFlag) { tmpvect = HitPoly.HitPosition; }
			tree.pos[i] = tmpvect;
			tree.rad[i] = VGet(0.0f, deg2rad(GetRand(360)), 0.0f);
			MV1SetPosition(tree.nears[i], tree.pos[i]);
			MV1SetPosition(tree.fars[i], tree.pos[i]);
			MV1SetRotationXYZ(tree.nears[i], tree.rad[i]);
			MV1DrawModel(tree.nears[i]);
			MV1SetMaterialDrawAlphaTestAll(tree.nears[i], TRUE, DX_CMP_GREATER, 128);
		}
		MV1DrawModel(m_model);
	ShadowMap_DrawEnd();
	return true;
}
void MAPS::set_camerapos(VECTOR pos, VECTOR vec, VECTOR up,float ratio){
	camera = pos;
	viewv = vec;
	upv = up;
	rat = ratio;
}
void MAPS::set_map_shadow_near(float vier_r){
	float shadow_dist = 15.0f * vier_r + 10.0f; if (shadow_dist <= 10.0f) { shadow_dist = 10.0f; }
	SetShadowMapDrawArea(shadow_near, VSub(camera, VScale(VGet(1.0f, 1.0f, 1.0f), shadow_dist)), VAdd(camera, VScale(VGet(1.0f, 1.0f, 1.0f), shadow_dist)));
	SetShadowMapDrawArea(shadow_seminear, VSub(camera, VScale(VGet(1.0f, 1.0f, 1.0f), shadow_dist * 2)), VAdd(camera, VScale(VGet(1.0f, 1.0f, 1.0f), shadow_dist * 2)));
}
void MAPS::draw_map_track(players*player) {
	SetDrawScreen(texn);
	for (int i = bone_wheel; i < player->ptr->frames - 4; i += 2) {
		if (player->Springs[i] >= -0.15f) {
			VECTOR tempvec = MV1GetFramePosition(player->obj, i);
			DrawRotaGraph((int)(groundx * (0.5f + tempvec.x / (float)map_x)), (int)(groundx * (0.5f - tempvec.z / (float)map_y)), 1.f*groundx / 1024 / 195.0f, player->yrad, texo, TRUE);
		}
	}

}
void MAPS::draw_map_model() {
	MV1DrawModel(m_model);
}
void MAPS::set_map_track() {
	MV1SetTextureGraphHandle(m_model, 1, texn, FALSE);
}
void MAPS::draw_map_sky(void){
	ClearDrawScreen();
	setcv(50.0f, 200.0f, camera, viewv, upv, 45.0f / rat);
	SetUseLighting(FALSE);
	SetFogEnable(FALSE);

	MV1SetPosition(sky_model, camera);
	MV1DrawModel(sky_model);
	DrawBillboard3D(VAdd(camera, VScale(VNorm(lightvec), -80.0f)), 0.5f, 0.5f, 10.0f, 0.0f, sky_sun, TRUE);

	SetFogEnable(TRUE);
	SetUseLighting(TRUE);

}
void MAPS::set_hitplayer(VECTOR pos) {
	for (int j = 0; j < treec; ++j) {
		if (tree.hit[j]) {
			if (VSize(VSub(tree.pos[j], pos)) <= 3.f) {
				tree.hit[j] = false;
				tree.rad[j].y = atan2(VSub(tree.pos[j], pos).x, VSub(tree.pos[j], pos).z);
			}
		}
		else {
			if (tree.rad[j].x <= deg2rad(85)) {
				MV1SetRotationXYZ(tree.nears[j], tree.rad[j]);
				differential(tree.rad[j].x, deg2rad(90), 0.002f);
			}
		}
	}
}
void MAPS::draw_trees() {
	int k;
	float per;
	VECTOR vect;
	looktree = 0;

	for (int j = 0; j < treec; ++j) {
		if (CheckCameraViewClip_Box(VAdd(tree.pos[j], VGet(-10, 0, -10)), VAdd(tree.pos[j], VGet(10, 10, 10)))) {
			tree.treesort[j] = pair(j, (float)map_x);
		}
		else {
			tree.treesort[j] = pair(j, VSize(VSub(tree.pos[j], camera)));
			++looktree;
		}
	}
	std::sort(tree.treesort.begin(), tree.treesort.end(), [](const pair& x, const pair& y) {return x.second > y.second; });

	for (auto& tt : tree.treesort) {
		if (tt.second == (float)map_x) { continue; }
		k = tt.first;
		if (tt.second < drawdist + 200) {
			if (tt.second < 20) { per = -0.5f + tt.second / 20.0f; }
			else { per = 1.0f; }
			if (tt.second > drawdist) { per = 1.0f - (tt.second - drawdist) / 200.0f; }
			if (per > 0) {
				MV1DrawModel(tree.nears[k]);
			}
		}
		if (tt.second > drawdist) {
			if (tt.second < drawdist + 200) { per = (tt.second - drawdist) / 200.0f; }
			else { per = 1.0f; }
			if (per > 0) {
				MV1SetOpacityRate(tree.fars[k], per);
				vect = VSub(tree.pos[k], camera);
				MV1SetRotationXYZ(tree.fars[k], VGet(0.0f, atan2(vect.x, vect.z), 0.0f));
				MV1DrawModel(tree.fars[k]);
			}
		}
	}
}
void MAPS::draw_grass(void) {
	DrawPolygonIndexed3D_UseVertexBuffer(VerBuf, IndexBuf, graph, TRUE);
}
void MAPS::delete_map(void) {
	MV1DeleteModel(m_model);
	MV1DeleteModel(sky_model);
	MV1DeleteModel(tree.mnear);
	MV1DeleteModel(tree.mfar);
	for (int j = 0; j < treec; ++j) {
		MV1DeleteModel(tree.nears[j]);
		MV1DeleteModel(tree.fars[j]);
	}
	tree.treesort.clear();
	DeleteGraph(graph);
	MV1DeleteModel(grass);
	DeleteGraph(texl);
	DeleteGraph(texm);
	tree.nears.clear();
	tree.fars.clear();
	tree.pos.clear();
	tree.rad.clear();
	tree.hit.clear();

	if (grassver != NULL) { delete[] grassver; grassver = NULL; }
	if (grassind != NULL) { delete[] grassind; grassind = NULL; }
	/*
	grassver.clear();
	grassind.clear();
	*/
}
//
UIS::UIS() {
	int j, i;
	std::string tempname, tempname2;
	WIN32_FIND_DATA win32fdt;
	HANDLE hFind;

	countries = 1;

	UI_main.resize(countries);/*改善*/
	SetUseASyncLoadFlag(TRUE);
		for (j = 0; j < 4; ++j) {
			tempname = "data/ui/ammo_" + std::to_string(j)+".bmp";
			ui_reload[j] = LoadGraph(tempname.c_str());
		}/*弾0,弾1,弾2,空弾*/
		hFind = FindFirstFile("data/ui/body/*.png", &win32fdt);
		if (hFind != INVALID_HANDLE_VALUE) {
			i = 0;
			j = 0;
			do {
				if (win32fdt.cFileName[0] == 'B') {
					tempname = "data/ui/body/";
					tempname += win32fdt.cFileName;
					UI_body.push_back(LoadGraph(tempname.c_str()));
				}
				if (win32fdt.cFileName[0] == 'T') {
					tempname = "data/ui/body/";
					tempname += win32fdt.cFileName;
					UI_turret.push_back(LoadGraph(tempname.c_str()));
				}
			} while (FindNextFile(hFind, &win32fdt));
		}//else{ return false; }
		FindClose(hFind);
		for (j = 0; j < countries; ++j) {
			tempname = "German";
			for (i = 0; i < 8; ++i) {
				tempname2 = "data/ui/" + tempname + "/" + std::to_string(i) + ".png";
				UI_main[j].ui_sight[i] = LoadGraph(tempname2.c_str());
			}
		}
	SetUseASyncLoadFlag(FALSE);
}
void UIS::draw_load(void){
	int i;
	float pers;
	int font18 = CreateFontToHandle(NULL, x_r(18), y_r(18 / 3), DX_FONTTYPE_ANTIALIASING);
	SetUseASyncLoadFlag(TRUE);
	int pad = LoadGraph("data/key.png");
	SetUseASyncLoadFlag(FALSE);
	int pp = GetASyncLoadNum();
	LONGLONG waits;
	while (ProcessMessage() == 0 && GetASyncLoadNum() != 0) {
		waits = GetNowHiPerformanceCount();
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		//
		pers = (float)(pp - GetASyncLoadNum()) / pp;
		DrawBox(x_r(0), y_r(1080-6), x_r(1920.f*pers), y_r(1080-3), GetColor(0, 255, 0), TRUE);
		DrawFormatStringToHandle(x_r(0), y_r(1080-24), GetColor(0, 255, 0), font18, "LOADING : %06.2f%%", pers * 100.f);
		//
		DrawExtendGraph(x_r(552), y_r(401), x_r(1367), y_r(679), pad, TRUE);
		i = 0;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255,   0,   0), font18, "%s : 前進", "W"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255,   0,   0), font18, "%s : 後退", "S"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255,   0,   0), font18, "%s : 左転", "A"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255,   0,   0), font18, "%s : 右転", "D"); i += 18;
		//DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255,   0,   0), font18, "%s : シフトアップ", "R"); i += 18;
		//DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255,   0,   0), font18, "%s : シフトダウン", "F"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : 砲昇", "↑"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : 砲降", "↓"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : 砲左", "←"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : 砲右", "→"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : 精密砲操作", "左CTRL"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : 照準", "左shift"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : レティクル上昇", "Z"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : レティクル下降", "X"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : ズームアウト", "C"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(255, 100,   0), font18, "%s : ズームイン", "V"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(  0, 200,   0), font18, "%s : 再装填", "Q"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor(  0, 200,   0), font18, "%s : 射撃", "space"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor( 50, 100, 255), font18, "%s : 指揮", "右shift"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor( 50, 100, 255), font18, "%s : 見回し/指揮", "マウス"); i += 18;
		DrawFormatStringToHandle(x_r(1367), y_r(401 + i), GetColor( 50, 100, 255), font18, "%s : 終了", "ESC"); i += 18;
		ScreenFlip();
		while (GetNowHiPerformanceCount() - waits < 1000000.0f / 60.f) {}
	}
	DeleteGraph(pad);
	DeleteFontToHandle(font18);

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	ScreenFlip();
}
void UIS::set_state(players* play) {
	pplayer = play;
}
void UIS::set_reco(void) {
	recs = 1.f;
}
void UIS::draw_sight(float posx, float posy, float ratio, float dist, int font) {
	DrawRotaGraph(x_r(960), y_r(540), (float)y_r(2), deg2rad(-dist / 20), UI_main[pplayer->ptr->countryc].ui_sight[1], TRUE);
	DrawRotaGraph((int)posx, (int)posy, (float)y_r(2) * ratio / 4.0f, 0, UI_main[pplayer->ptr->countryc].ui_sight[2], TRUE);
	DrawRotaGraph(x_r(960), y_r(540), (float)y_r(2), 0, UI_main[pplayer->ptr->countryc].ui_sight[0], TRUE);
	DrawExtendGraph(0, 0, dispx, dispy, UI_main[pplayer->ptr->countryc].ui_sight[7], TRUE);
	DrawFormatStringToHandle(x_r(1056), y_r(594), GetColor(255, 255, 255), font, "[%03d]", (int)dist);
	DrawFormatStringToHandle(x_r(1056), y_r(648), GetColor(255, 255, 255), font, "[x%02.1f]", ratio);
}
void UIS::draw_ui(int selfammo, float y_v) {
	float pers;
	/*跳弾*/
	if (recs >= 0.01f) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(recs * 128.0f));
		DrawBox(0, 0, dispx, dispy, GetColor(255, 255, 255), TRUE);
		recs *= 0.9f;
	}
	/*弾*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	if (pplayer->loadcnt[0] > 0) {
		DrawRotaGraph(x_r(2112 - (int)(384 * pplayer->loadcnt[0] / pplayer->ptr->reloadtime[0])), y_r(64), (double)x_r(40) / 40.0, 0.0, ui_reload[pplayer->ammotype], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(128.0f * pow(1.0f - (float)pplayer->loadcnt[0] / (float)pplayer->ptr->reloadtime[0], 10)));
		if (selfammo == 0) {
			DrawRotaGraph(x_r(1536), y_r(64), (double)x_r(40) / 40.0, 0.0, ui_reload[3], TRUE);
		}
		else {
			DrawRotaGraph(x_r(1536), y_r(64), (double)x_r(40) / 40.0, 0.0, ui_reload[(pplayer->ammotype - 1) % 3], TRUE);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	if (pplayer->loadcnt[0] == 0) {
		DrawRotaGraph(x_r(1536), y_r(64), (double)x_r(40) / 40.0, 0.0, ui_reload[pplayer->ammotype], TRUE);
	}
	DrawRotaGraph(x_r(1728 - (int)(192 * pplayer->loadcnt[0] / pplayer->ptr->reloadtime[0])), y_r(64), (double)x_r(40) / 40.0, 0.0, ui_reload[pplayer->ammotype], TRUE);
	DrawRotaGraph(x_r(1760), y_r(128), (double)x_r(40) / 40.0, 0.0, ui_reload[(pplayer->ammotype + 1) % 3], TRUE);
	DrawRotaGraph(x_r(1792), y_r(192), (double)x_r(40) / 40.0, 0.0, ui_reload[(pplayer->ammotype + 2) % 3], TRUE);
	/*速度計*/
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawExtendGraph(x_r(0), y_r(888), x_r(192), y_r(1080), UI_main[pplayer->ptr->countryc].ui_sight[3], TRUE);
	DrawRotaGraph(x_r(96), y_r(984), x_r(192) / 152, deg2rad(120.0f * pplayer->speed / pplayer->ptr->spdflont[3] - 60.f), UI_main[pplayer->ptr->countryc].ui_sight[4], TRUE);

	SetDrawArea(x_r(192), y_r(892), x_r(192 + 40), y_r(892 + 54));
	DrawRotaGraph(x_r(192 + 40 / 2), y_r(892 + 54 / 2 + (int)(54.0f * gearf)), (double)x_r(40) / 40.0, 0.f, UI_main[pplayer->ptr->countryc].ui_sight[5], TRUE);
	SetDrawArea(x_r(0), y_r(0), x_r(1920), y_r(1080));

	DrawExtendGraph(x_r(192), y_r(892 - 4), x_r(232), y_r(950), UI_main[pplayer->ptr->countryc].ui_sight[6], TRUE);
	differential(gearf, (float)pplayer->gear, 0.1f);

	for (int i = 0; i < UI_body.size(); ++i) {
		if (i >= 1 && i <= 2) {
			pers = 1.f - (float)pplayer->HP[5 + i - 1] / 100.f;
			if (pers >= 0.8f) { SetDrawBright(50, 50, 255); }
			else { SetDrawBright((int)(255.f * sin(pers*DX_PI_F / 2)), (int)(255.f * cos(pers*DX_PI_F / 2)), 0); }
		}
		else {
			SetDrawBright(255, 255, 255);
		}
		DrawRotaGraph(x_r(392), y_r(980), (double)x_r(40) / 40.0, -y_v + pplayer->yrad, UI_body[i], TRUE);
	}

	for (int i = 0; i < UI_turret.size(); ++i) {
		if (i == 0) {
			pers = 1.f - (float)pplayer->HP[4 + i] / 100.f;
			if (pers >= 0.8f) { SetDrawBright(50, 50, 255); }
			else { SetDrawBright((int)(255.f * sin(pers*DX_PI_F / 2)), (int)(255.f * cos(pers*DX_PI_F / 2)), 0); }
		}
		else { SetDrawBright(255, 255, 255); }
		DrawRotaGraph(x_r(392), y_r(980), (double)x_r(40) / 40.0, -y_v + pplayer->yrad + pplayer->gunrad.x, UI_turret[i], TRUE);
	}
	//DrawFormatString(x_r(1056), y_r(594), GetColor(255, 255, 255), "[%03d][%03d]", UI_body.size(),UI_turret.size());
}

void UIS::put_way(void){
		waypoint = GetNowHiPerformanceCount();
		seldeb = 0;
}
void UIS::end_way(void){
	if (seldeb < 6) {
		waydeb[seldeb] = (float)(GetNowHiPerformanceCount() - waypoint) / 1000.0f;
		seldeb++;
	}
}
void UIS::debug(float fps, float time) {
	int j, i;
	deb[0][0] = time;
	for (j = 60 - 1; j >= 1; --j) { deb[j][0] = deb[j - 1][0]; }


	for (i = 0; i < 6; ++i) {
		if (seldeb - 1 <= i) { waydeb[i] = waydeb[seldeb-1]; }
		deb[0][i + 1] = waydeb[i];
		for (j = 60 - 1; j >= 1; --j) { deb[j][i + 1] = deb[j - 1][i + 1]; }
	}

	for (j = 0; j < 60 - 1; ++j) {
		for (i = 0; i < 6; ++i) {
			DrawLine(100 + j * 5, 100 + (int)(200.f - deb[j][i + 1] * 10.f), 100 + (j + 1) * 5, 100 + (int)(200.f - deb[j + 1][i + 1] * 10.f), GetColor(50, 50, 128+127*i/6));
		}
		DrawLine(100 + j * 5, 100 + (int)(200.f - deb[j][0] * 10.f), 100 + (j + 1) * 5, 100 + (int)(200.f - deb[j + 1][0] * 10.f), GetColor(255, 255, 0));
	}
	DrawLine(100, 100 + 200-166, 100 + 60 * 5, 100 + 200 - 166, GetColor(0, 255, 0));
	DrawBox(100, 100 + 0, 100 + 60 * 5, 100 + 200, GetColor(255, 0, 0), FALSE);
	DrawFormatString(100, 100 + 0, GetColor(255, 255, 255), "%05.2ffps (%.2fms)", fps, time);
	DrawFormatString(100, 100 + 18, GetColor(255, 255, 255), "%d(%.2fms)", 0, waydeb[0]);
	for (j = 1; j < 6; ++j) {
		DrawFormatString(100, 100 + 18 + j * 18, GetColor(255, 255, 255), "%d(%.2fms)", j, waydeb[j] - waydeb[j - 1]);
	}
}
//
void setcv(float neard, float fard, VECTOR cam, VECTOR view, VECTOR up, float fov) {
	SetCameraNearFar(neard, fard);
	SetCameraPositionAndTargetAndUpVec(cam, view, up);
	SetupCamera_Perspective(deg2rad(fov));
	Set3DSoundListenerPosAndFrontPosAndUpVec(cam, view, up);
}
void getdist(VECTOR *startpos, VECTOR vector, float *dist, float speed, float fps) {
	if (*dist >= 2000.0f) { *dist = 2000.0f; }
	if (*dist <= 100.0f) { *dist = 100.0f; }
	for (int z = 0; z < (int)(fps / 1000.0f * (*dist)); ++z) {
		*startpos = VAdd(*startpos, VScale(vector, speed / fps));
		vector.y += M_GR / 2.0f / fps / fps;
		speed -= 5.f / fps;
	}
}
//
void set_effect(EffectS * efh, VECTOR pos, VECTOR nor) {
	efh->efflug = true;
	efh->effpos = pos;
	efh->effnor = nor;
}
void set_pos_effect(EffectS *efh, int handle) {
	if (efh->efflug) {
		efh->efhandle = PlayEffekseer3DEffect(handle);
		SetPosPlayingEffekseer3DEffect(efh->efhandle, efh->effpos.x, efh->effpos.y, efh->effpos.z);
		SetRotationPlayingEffekseer3DEffect(efh->efhandle, atan2(efh->effnor.y, sqrt(pow(efh->effnor.x, 2) + pow(efh->effnor.z, 2))), atan2(-efh->effnor.x, -efh->effnor.z), 0);
		efh->efflug = false;
	}
	//IsEffekseer3DEffectPlaying(player[0].effcs[i].efhandle)
}
//
void set_normal(float* xnor, float* znor, int maphandle, VECTOR position) {
	float temp[2];
	MV1_COLL_RESULT_POLY HitPoly2;

	HitPoly2 = MV1CollCheck_Line(maphandle, 0, VAdd(position, VGet(0.0f, 2.0f, -0.5f)), VAdd(position, VGet(0.0f, -2.0f, -0.5f)));
	if (HitPoly2.HitFlag) { temp[0] = HitPoly2.HitPosition.y; }
	else { temp[0] = -9999.0f; }
	HitPoly2 = MV1CollCheck_Line(maphandle, 0, VAdd(position, VGet(0.0f, 2.0f, 0.5f)), VAdd(position, VGet(0.0f, -2.0f, 0.5f)));
	if (HitPoly2.HitFlag) { temp[1] = HitPoly2.HitPosition.y; }
	else { temp[1] = -9999.0f; }
	if (temp[0] != -9999.0 && temp[1] != -9999.0) { differential(*xnor, atan2(temp[0] - temp[1], 1.0f), 0.05f); }/*X*/

	HitPoly2 = MV1CollCheck_Line(maphandle, 0, VAdd(position, VGet(0.5f, 2.0f, 0.0f)), VAdd(position, VGet(0.5f, -2.0f, 0.0f)));
	if (HitPoly2.HitFlag) { temp[0] = HitPoly2.HitPosition.y; }
	else { temp[0] = -9999.0f; }
	HitPoly2 = MV1CollCheck_Line(maphandle, 0, VAdd(position, VGet(-0.5f, 2.0f, 0.0f)), VAdd(position, VGet(-0.5f, -2.0f, 0.0f)));
	if (HitPoly2.HitFlag) { temp[1] = HitPoly2.HitPosition.y; }
	else { temp[1] = -9999.0f; }
	if (temp[0] != -9999.0 && temp[1] != -9999.0) { differential(*znor, atan2(temp[0] - temp[1], 1.0f), 0.05f); }/*Z*/
}
bool get_reco(players* play, players* tgt, int i, int gun_s) {
	int colmesh, hitnear, k, f = 0;
	MV1_COLL_RESULT_POLY HitPoly;
	float tmpf[2];
	//主砲
	hitnear = -1;
	if (gun_s == 0) {
		//空間装甲、モジュール
		for (colmesh = 0; colmesh < tgt->ptr->colmeshes; ++colmesh) {
			HitPoly = MV1CollCheck_Line(tgt->colobj, -1, play->Ammo[i].repos, play->Ammo[i].pos, colmesh);
			if (HitPoly.HitFlag) {
				tgt->hitssort[colmesh] = pair(colmesh, VSize(VSub(HitPoly.HitPosition, play->Ammo[i].repos)));
				f++;
			}
			else {
				tgt->hitssort[colmesh] = pair(colmesh, 9999.f);
			}
		}
		if (f == 0) { return false; }
		std::sort(tgt->hitssort.begin(), tgt->hitssort.end(), [](const pair& x, const pair& y) {return x.second < y.second; });
		for (auto& tt : tgt->hitssort) {
			k = tt.first;
			if (k >= 4) {
				if (tgt->HP[k] > 0) {
					//空間装甲 //モジュール
					if (k != 4) {
						HitPoly = MV1CollCheck_Line(tgt->colobj, -1, play->Ammo[i].repos, play->Ammo[i].pos, k);
						if (HitPoly.HitFlag) {
							set_effect(&(play->effcs[ef_reco]), HitPoly.HitPosition, HitPoly.Normal);
							tgt->HP[k] -= 50; if (tgt->HP[k] <= 0) { tgt->HP[k] = 0; } play->Ammo[i].pene /= 2.0f; play->Ammo[i].speed /= 2.f;
						}
					}
				}
			}
			else {
				hitnear = k; break;
			}
			if (tt.second == 9999.f) { break; }
		}
		if (hitnear != -1) {
			HitPoly = MV1CollCheck_Line(tgt->colobj, -1, play->Ammo[i].repos, play->Ammo[i].pos, hitnear);
			MV1SetFrameUserLocalMatrix(tgt->colobj, 9 + 0 + 3 * tgt->hitbuf, MMult(MGetTranslate(HitPoly.HitPosition), MInverse(tgt->ps_m)));
			MV1SetFrameUserLocalMatrix(tgt->colobj, 9 + 1 + 3 * tgt->hitbuf, MMult(MGetTranslate(VAdd(HitPoly.Normal, HitPoly.HitPosition)), MInverse(tgt->ps_m)));
			MV1SetFrameUserLocalMatrix(tgt->colobj, 9 + 2 + 3 * tgt->hitbuf, MMult(MGetTranslate(VAdd(VCross(HitPoly.Normal, play->Ammo[i].vec), HitPoly.HitPosition)), MInverse(tgt->ps_m)));
			set_effect(&(play->effcs[ef_reco]), HitPoly.HitPosition, HitPoly.Normal);
			if (play->Ammo[i].pene > tgt->ptr->armer[hitnear] * (1.0f / abs(VDot(VNorm(play->Ammo[i].vec), HitPoly.Normal)))) {
				if (tgt->HP[0] != 0) {
					PlaySoundMem(tgt->se[29 + GetRand(1)], DX_PLAYTYPE_BACK, TRUE);
					set_effect(&(play->effcs[ef_bomb]), MV1GetFramePosition(tgt->obj, bone_engine), VGet(0, 0, 0));
					set_effect(&(play->effcs[ef_smoke1]), MV1GetFramePosition(tgt->obj, bone_engine), VGet(0, 0, 0));
					if (play->hitadd == false) { play->hitadd = true; }
				}
				play->Ammo[i].flug = 0;
				tgt->HP[0] = 0;
				tgt->usepic[tgt->hitbuf] = 0;
			}
			else {
				PlaySoundMem(tgt->se[10 + GetRand(16)], DX_PLAYTYPE_BACK, TRUE);
				if (tgt->recoadd == false) {
					tgt->recorad = atan2(HitPoly.HitPosition.x - tgt->pos.x, HitPoly.HitPosition.z - tgt->pos.z);
					tgt->recoadd = true;
				}
				play->Ammo[i].vec = VAdd(play->Ammo[i].vec, VScale(HitPoly.Normal, VDot(play->Ammo[i].vec, HitPoly.Normal) * -2.0f));
				play->Ammo[i].pos = VAdd(HitPoly.HitPosition, VScale(play->Ammo[i].vec, 0.01f));

				play->Ammo[i].pene /= 2.0f; play->Ammo[i].speed /= 2.f;

				tgt->usepic[tgt->hitbuf] = 1;
			}
			MV1SetScale(tgt->hitpic[tgt->hitbuf], VGet(play->ptr->ammosize*100.f*(1.0f / abs(VDot(VNorm(play->Ammo[i].vec), HitPoly.Normal))), play->ptr->ammosize*100.f, play->ptr->ammosize*100.f));//
			tgt->hitbuf++; tgt->hitbuf %= 3;
		}
	}
	//同軸機銃
	else {
		tmpf[0] = play->Ammo[i].speed;
		for (colmesh = 0; colmesh < tgt->ptr->colmeshes; ++colmesh) {
			if (colmesh >= 5) { if (tgt->HP[colmesh] == 0) { continue; } }
			HitPoly = MV1CollCheck_Line(tgt->colobj, -1, play->Ammo[i].repos, play->Ammo[i].pos, colmesh);
			if (HitPoly.HitFlag) { tmpf[1] = VSize(VSub(HitPoly.HitPosition, play->Ammo[i].repos)); if (tmpf[1] <= tmpf[0]) { tmpf[0] = tmpf[1]; hitnear = colmesh; } }
		}
		if (hitnear != -1) {
			HitPoly = MV1CollCheck_Line(tgt->colobj, -1, play->Ammo[i].repos, play->Ammo[i].pos, hitnear);
			set_effect(&(play->effcs[ef_reco2]), HitPoly.HitPosition, HitPoly.Normal);
			PlaySoundMem(tgt->se[10 + GetRand(16)], DX_PLAYTYPE_BACK, TRUE);
			play->Ammo[i].vec = VAdd(play->Ammo[i].vec, VScale(HitPoly.Normal, VDot(play->Ammo[i].vec, HitPoly.Normal) * -2.0f));
			play->Ammo[i].pos = VAdd(HitPoly.HitPosition, VScale(play->Ammo[i].vec, 0.01f));
		}
	}
	if (hitnear != -1) { return true; }
	return false;
}
void set_gunrad(players *play, float rat_r){
	for (int i = 0; i < 4; ++i) {
		if ((play->move & (KEY_TURNLFT << i)) != 0) {
			switch (i) {
			case 0:
				play->gunrad.x -= play->ptr->gun_RD / rat_r;//
				if (!play->ptr->gun_lim_LR) { if (play->gunrad.x >= play->ptr->gun_lim_[0]) { play->gunrad.x = play->ptr->gun_lim_[0]; } }
				break;
			case 1:
				play->gunrad.x += play->ptr->gun_RD / rat_r;//
				if (!play->ptr->gun_lim_LR) { if (play->gunrad.x <= play->ptr->gun_lim_[1]) { play->gunrad.x = play->ptr->gun_lim_[1]; } }
				break;
			case 2:
				play->gunrad.y += (play->ptr->gun_RD / 2.f) / rat_r;//
				if (play->gunrad.y >= play->ptr->gun_lim_[2]) { play->gunrad.y = play->ptr->gun_lim_[2]; }
				break;
			case 3:
				play->gunrad.y -= (play->ptr->gun_RD / 2.f) / rat_r;//
				if (play->gunrad.y <= play->ptr->gun_lim_[3]) { play->gunrad.y = play->ptr->gun_lim_[3]; }
				break;
			}
		}
	}
}
bool set_shift(players * play){
	int gearrec = play->gear;
	/*自動変速機*/
	if (play->gear > 0 && play->gear <= 3) { if (play->flont >= play->ptr->spdflont[play->gear - 1] * 0.9) { ++play->gear; } }
	if (play->gear < 0 && play->gear >= -3) { if (play->back <= play->ptr->spdback[-play->gear - 1] * 0.9) { --play->gear; } }
	if ((play->move & KEY_GOFLONT) == 0 && play->gear > 0) { --play->gear; }
	else if ((play->move & KEY_GOBACK_) == 0 && play->gear < 0) { ++play->gear; }
	/*変速*/
	/*
	if (play->gear > 0 && play->gear <= 3) {
		if (CheckHitKey(KEY_INPUT_R) != 0) { ++play->gearu; if (play->gearu == 1) { ++play->gear; } }
		else { play->gearu = 0; }
	}
	if (play->gear < 0 && play->gear >= -3) {
		if (CheckHitKey(KEY_INPUT_F) != 0) { ++play->geard; if (play->geard == 1) { --play->gear; } }
		else { play->geard = 0; }
	}
	*/
	/**/
	if (play->gear == 0) {
		if ((play->move & KEY_GOFLONT) != 0) { ++play->gear; }
		if ((play->move & KEY_GOBACK_) != 0) { --play->gear; }
	}
	if (play->gear != gearrec) { return true; }
	return false;
}
