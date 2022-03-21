const Note Note::Small() { return Note("01"); }
const Note Note::SmallUp() { return Note("02"); }
const Note Note::SmallDown() { return Note("03"); }
const Note Note::Medium1() { return Note("04"); }
const Note Note::Medium1Up() { return Note("05"); }
const Note Note::Medium1Down() { return Note("06"); }
const Note Note::Medium2() { return Note("07"); }
const Note Note::Medium2Up() { return Note("08"); }
const Note Note::Medium2Down() { return Note("09"); }
const Note Note::Large1() { return Note("0A"); }
const Note Note::Large2() { return Note("0B"); }
const Note Note::Raider() { return Note("0C"); }
const Note Note::Hammer() { return Note("0D"); }
const Note Note::Gemini() { return Note("0E"); }
const Note Note::Hold() { return Note("0F"); }
const Note Note::Masher() { return Note("0G"); }
const Note Note::Gear() { return Note("0H"); }
const Note Note::Speed1Both() { return Note("0O"); }
const Note Note::Speed2Both() { return Note("0P"); }
const Note Note::Speed3Both() { return Note("0Q"); }
const Note Note::Speed1Low() { return Note("0R"); }
const Note Note::Speed2Low() { return Note("0S"); }
const Note Note::Speed3Low() { return Note("0T"); }
const Note Note::Speed1High() { return Note("0U"); }
const Note Note::Speed2High() { return Note("0V"); }
const Note Note::Speed3High() { return Note("0W"); }
const Note Note::BossMelee1() { return Note("11"); }
const Note Note::BossMelee2() { return Note("12"); }
const Note Note::BossProjectile1() { return Note("13"); }
const Note Note::BossProjectile2() { return Note("14"); }
const Note Note::BossProjectile3() { return Note("15"); }
const Note Note::BossMasher1() { return Note("16"); }
const Note Note::BossMasher2() { return Note("17"); }
const Note Note::BossGear() { return Note("18"); }
const Note Note::BossEntrance() { return Note("1A"); }
const Note Note::BossExit() { return Note("1B"); }
const Note Note::BossReadyPhase1() { return Note("1C"); }
const Note Note::BossEndPhase1() { return Note("1D"); }
const Note Note::BossReadyPhase2() { return Note("1E"); }
const Note Note::BossEndPhase2() { return Note("1F"); }
const Note Note::BossSwapPhase1_2() { return Note("1G"); }
const Note Note::BossSwapPhase2_1() { return Note("1H"); }
const Note Note::HideNotes() { return Note("1J"); }
const Note Note::UnhideNotes() { return Note("1K"); }
const Note Note::HideBoss() { return Note("1L"); }
const Note Note::Unhideboss() { return Note("1M"); }
const Note Note::SceneSwitchSpaceStation() { return Note("1O"); }
const Note Note::SceneSwitchRetrocity() { return Note("1P"); }
const Note Note::SceneSwitchCastle() { return Note("1Q"); }
const Note Note::SceneSwitchRainyNight() { return Note("1R"); }
const Note Note::SceneSwitchCandyland() { return Note("1S"); }
const Note Note::SceneSwitchOriental() { return Note("1T"); }
const Note Note::SceneSwitchGrooveCoaster() { return Note("1U"); }
const Note Note::SceneSwitchTouhou() { return Note("1V"); }
const Note Note::SceneSwitchDJMAX() { return Note("1W"); }
const Note Note::PItem() { return Note("20"); }
const Note Note::Ghost() { return Note("21"); }
const Note Note::Heart() { return Note("22"); }
const Note Note::Note_() { return Note("23"); }
const Note Note::HideBackground() { return Note("25"); }
const Note Note::UnhideBackground() { return Note("26"); }
const Note Note::ScreenScrollUP() { return Note("27"); }
const Note Note::ScreenScrollDOWN() { return Note("28"); }
const Note Note::ScreenScrollOFF() { return Note("29"); }
const Note Note::ScanlineRipplesON() { return Note("2A"); }
const Note Note::ScanlineRipplesOFF() { return Note("2B"); }
const Note Note::ChromaticAberrationON() { return Note("2C"); }
const Note Note::ChromaticAberrationOFF() { return Note("2D"); }
const Note Note::VignetteON() { return Note("2E"); }
const Note Note::VignetteOFF() { return Note("2F"); }
const Note Note::TVstaticON() { return Note("2G"); }
const Note Note::TVstaticOFF() { return Note("2H"); }
const Note Note::FlashbangSTART() { return Note("2I"); }
const Note Note::FlashbangMID() { return Note("2J"); }
const Note Note::FlashbangEND() { return Note("2K"); }
const Note Note::MediumBullet() { return Note("30"); }
const Note Note::MediumBulletUp() { return Note("31"); }
const Note Note::MediumBulletDown() { return Note("32"); }
const Note Note::MediumBulletLaneshift() { return Note("33"); }
const Note Note::SmallBullet() { return Note("34"); }
const Note Note::SmallBulletUp() { return Note("35"); }
const Note Note::SmallBulletDown() { return Note("36"); }
const Note Note::SmallBulletLaneshift() { return Note("37"); }
const Note Note::LargeBullet() { return Note("38"); }
const Note Note::LargeBulletUp() { return Note("39"); }
const Note Note::LargeBulletDown() { return Note("3A"); }
const Note Note::LargeBulletLaneshift() { return Note("3B"); }
const Note Note::BossBullet1() { return Note("3C"); }
const Note Note::BossBullet1Laneshift() { return Note("3D"); }
const Note Note::BossBullet2() { return Note("3E"); }
const Note Note::BossBullet2Laneshift() { return Note("3F"); }
const map<string, string> names = {
	{"01", "Small"},
	{"02", "Small (Up)"},
	{"03", "Small (Down)"},
	{"04", "Medium 1"},
	{"05", "Medium 1 (Up)"},
	{"06", "Medium 1 (Down)"},
	{"07", "Medium 2"},
	{"08", "Medium 2 (Up)"},
	{"09", "Medium 2 (Down)"},
	{"0A", "Large 1"},
	{"0B", "Large 2"},
	{"0C", "Raider"},
	{"0D", "Hammer"},
	{"0E", "Gemini"},
	{"0F", "Hold"},
	{"0G", "Masher"},
	{"0H", "Gear"},
	{"0O", "Speed 1 (Both)"},
	{"0P", "Speed 2 (Both)"},
	{"0Q", "Speed 3 (Both)"},
	{"0R", "Speed 1 (Low)"},
	{"0S", "Speed 2 (Low)"},
	{"0T", "Speed 3 (Low)"},
	{"0U", "Speed 1 (High)"},
	{"0V", "Speed 2 (High)"},
	{"0W", "Speed 3 (High)"},
	{"11", "Boss Melee 1"},
	{"12", "Boss Melee 2"},
	{"13", "Boss Projectile 1"},
	{"14", "Boss Projectile 2"},
	{"15", "Boss Projectile 3"},
	{"16", "Boss Masher 1"},
	{"17", "Boss Masher 2"},
	{"18", "Boss Gear"},
	{"1A", "Boss Entrance"},
	{"1B", "Boss Exit"},
	{"1C", "Boss Ready Phase 1"},
	{"1D", "Boss End Phase 1"},
	{"1E", "Boss Ready Phase 2"},
	{"1F", "Boss End Phase 2"},
	{"1G", "Boss Swap Phase 1-2"},
	{"1H", "Boss Swap Phase 2-1"},
	{"1J", "Hide Notes"},
	{"1K", "Unhide Notes"},
	{"1L", "Hide Boss"},
	{"1M", "Unhide boss"},
	{"1O", "Scene Switch (Space Station)"},
	{"1P", "Scene Switch (Retrocity)"},
	{"1Q", "Scene Switch (Castle)"},
	{"1R", "Scene Switch (Rainy Night)"},
	{"1S", "Scene Switch (Candyland)"},
	{"1T", "Scene Switch (Oriental)"},
	{"1U", "Scene Switch (Groove Coaster)"},
	{"1V", "Scene Switch (Touhou)"},
	{"1W", "Scene Switch (DJMAX)"},
	{"20", "P Item"},
	{"21", "Ghost"},
	{"22", "Heart"},
	{"23", "Note"},
	{"25", "Hide Background"},
	{"26", "Unhide Background"},
	{"27", "Screen Scroll UP"},
	{"28", "Screen Scroll DOWN"},
	{"29", "Screen Scroll OFF"},
	{"2A", "Scanline Ripples ON"},
	{"2B", "Scanline Ripples OFF"},
	{"2C", "Chromatic Aberration ON"},
	{"2D", "Chromatic Aberration OFF"},
	{"2E", "Vignette ON"},
	{"2F", "Vignette OFF"},
	{"2G", "TV static ON"},
	{"2H", "TV static OFF"},
	{"2I", "Flashbang START"},
	{"2J", "Flashbang MID"},
	{"2K", "Flashbang END"},
	{"30", "Medium Bullet"},
	{"31", "Medium Bullet (Up)"},
	{"32", "Medium Bullet (Down)"},
	{"33", "Medium Bullet (Laneshift)"},
	{"34", "Small Bullet"},
	{"35", "Small Bullet (Up)"},
	{"36", "Small Bullet (Down)"},
	{"37", "Small Bullet (Laneshift)"},
	{"38", "Large Bullet"},
	{"39", "Large Bullet (Up)"},
	{"3A", "Large Bullet (Down)"},
	{"3B", "Large Bullet (Laneshift)"},
	{"3C", "Boss Bullet 1"},
	{"3D", "Boss Bullet 1 (Laneshift)"},
	{"3E", "Boss Bullet 2"},
	{"3F", "Boss Bullet 2 (Laneshift)"}
};
const static Note Small();
const static Note SmallUp();
const static Note SmallDown();
const static Note Medium1();
const static Note Medium1Up();
const static Note Medium1Down();
const static Note Medium2();
const static Note Medium2Up();
const static Note Medium2Down();
const static Note Large1();
const static Note Large2();
const static Note Raider();
const static Note Hammer();
const static Note Gemini();
const static Note Hold();
const static Note Masher();
const static Note Gear();
const static Note Speed1Both();
const static Note Speed2Both();
const static Note Speed3Both();
const static Note Speed1Low();
const static Note Speed2Low();
const static Note Speed3Low();
const static Note Speed1High();
const static Note Speed2High();
const static Note Speed3High();
const static Note BossMelee1();
const static Note BossMelee2();
const static Note BossProjectile1();
const static Note BossProjectile2();
const static Note BossProjectile3();
const static Note BossMasher1();
const static Note BossMasher2();
const static Note BossGear();
const static Note BossEntrance();
const static Note BossExit();
const static Note BossReadyPhase1();
const static Note BossEndPhase1();
const static Note BossReadyPhase2();
const static Note BossEndPhase2();
const static Note BossSwapPhase1_2();
const static Note BossSwapPhase2_1();
const static Note HideNotes();
const static Note UnhideNotes();
const static Note HideBoss();
const static Note Unhideboss();
const static Note SceneSwitchSpaceStation();
const static Note SceneSwitchRetrocity();
const static Note SceneSwitchCastle();
const static Note SceneSwitchRainyNight();
const static Note SceneSwitchCandyland();
const static Note SceneSwitchOriental();
const static Note SceneSwitchGrooveCoaster();
const static Note SceneSwitchTouhou();
const static Note SceneSwitchDJMAX();
const static Note PItem();
const static Note Ghost();
const static Note Heart();
const static Note Note_();
const static Note HideBackground();
const static Note UnhideBackground();
const static Note ScreenScrollUP();
const static Note ScreenScrollDOWN();
const static Note ScreenScrollOFF();
const static Note ScanlineRipplesON();
const static Note ScanlineRipplesOFF();
const static Note ChromaticAberrationON();
const static Note ChromaticAberrationOFF();
const static Note VignetteON();
const static Note VignetteOFF();
const static Note TVstaticON();
const static Note TVstaticOFF();
const static Note FlashbangSTART();
const static Note FlashbangMID();
const static Note FlashbangEND();
const static Note MediumBullet();
const static Note MediumBulletUp();
const static Note MediumBulletDown();
const static Note MediumBulletLaneshift();
const static Note SmallBullet();
const static Note SmallBulletUp();
const static Note SmallBulletDown();
const static Note SmallBulletLaneshift();
const static Note LargeBullet();
const static Note LargeBulletUp();
const static Note LargeBulletDown();
const static Note LargeBulletLaneshift();
const static Note BossBullet1();
const static Note BossBullet1Laneshift();
const static Note BossBullet2();
const static Note BossBullet2Laneshift();
