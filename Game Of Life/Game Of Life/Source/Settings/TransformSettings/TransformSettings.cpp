namespace TransformSettings
{
	char SlowMultiplier = 1;
	char DefaultMultiplier = 10;
	char FastMultiplier = 100;
	char SpeedMultiplier = DefaultMultiplier;

	bool ZoomOnMouse = true;
	unsigned short Zoom = 0;
	unsigned short MaxZoom = 0xFFFF - FastMultiplier;
	unsigned short MaxUiZoom = 4000;
	unsigned short MinUiZoom = 0;

	long long PanX = 0;
	long long PanY = 0;
	long long PanOffsetX = 0;
	long long PanOffsetY = 0;
	long long PanMultiplier = 90000000;
	long long MaxPan = 10000000000;
	long long MaxUiPan = MaxPan / 2;
	long long MinUiPan = -MaxUiPan;
}
