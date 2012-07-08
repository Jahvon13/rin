#include "inc/main.h"
//#include "inc/gz.h"
//#include "inc/zlibInterface.h"
#include <dirent.h>

char path_inzip[PATH_MAX]={0};


// �g���q�Ǘ��p
const struct {
	char *szExt;
	int nExtId;
} stExtentions[] = {
 "gb",EXT_GB,
 "gbc",EXT_GB,
 "sgb",EXT_GB,
 "gz",EXT_GZ,
 "zip",EXT_ZIP,
 "tch",EXT_TCH,
 "png", EXT_IMG,
 NULL, EXT_UNKNOWN
};
int getExtId(const char *szFilePath) {
	char *pszExt;

	if ((pszExt = strrchr(szFilePath, '.'))) {
		pszExt++;
		int i;
		for (i = 0; stExtentions[i].nExtId != EXT_UNKNOWN; i++) {
			if (!strcasecmp(stExtentions[i].szExt,pszExt)) {
				return stExtentions[i].nExtId;
			}
		}
	}

	return EXT_UNKNOWN;
}

// Unzip �Ή� by ruka

// �R�[���o�b�N�󂯓n���p
typedef struct {
	byte *p_rom_image;			// pointer to rom image
	long rom_size;				// rom size
	char szFileName[PATH_MAX];		// extracted file name
}ROM_INFO, *LPROM_INFO;

// Unzip �R�[���o�b�N
/*int funcUnzipCallback(int nCallbackId, unsigned long ulExtractSize, unsigned long ulCurrentPosition,
                      const void *pData, unsigned long ulDataSize, unsigned long ulUserData)
{
    const char *pszFileName;
    int nExtId;
    const unsigned char *pbData;
    LPROM_INFO pRomInfo = (LPROM_INFO)ulUserData;

    switch(nCallbackId) {
    case UZCB_FIND_FILE:
		pszFileName = (const char *)pData;
		
		if (path_inzip[0] && strcmp(pszFileName,path_inzip))
			break;
		
		nExtId = getExtId(pszFileName);
		// �g���q��GB�EGBC�ESGB�Ȃ�W�J
		if (nExtId == EXT_GB) {
			// �W�J���閼�O�Arom size���o���Ă���
			strcpy(pRomInfo->szFileName, pszFileName);
			pRomInfo->rom_size = ulExtractSize;
			return UZCBR_OK;
		}
        break;
    case UZCB_EXTRACT_PROGRESS:
		pbData = (const unsigned char *)pData;
		// �W�J���ꂽ�f�[�^���i�[���悤
		memcpy(pRomInfo->p_rom_image + ulCurrentPosition, pbData, ulDataSize);
		return UZCBR_OK;
        break;
    default: // unknown...
		pgFillvram(RGBA_8(255,0,0));
		pgPrintf(0,0,0xFFFF, "Unzip fatal error.");
		pgScreenFlip();
        break;
    }
    return UZCBR_PASS;
}*/

// load rom image by ruka
long load_rom(const char *szRomPath)
{
	char name[MAX_NAME], *p, tmp[PATH_MAX];
	long lReadSize=0;
	ROM_INFO stRomInfo;
	int nRet;
	int nExtId = getExtId(szRomPath);

	switch(nExtId) {
	case EXT_GB:{	// "gb" "gbc" "sgb"
		p = strrchr(szRomPath, '/');
		if (!p)
			return 0;
		strcpy(name, p+1);

		FILE * fd = fopen(szRomPath, "r");
		lReadSize = fread(rom_image, 1, MAX_ROM_SIZE, fd);
		fclose(fd);
		break;

	}/*case EXT_GZ:{	// "gz"
		gzFile fd = gzopen(szRomPath, "r");
		if (!gzGetOrigName(szRomPath, name, fd)){
			gzclose(fd);
			return 0;
		}else if (getExtId(name)!=EXT_GB){
			gzclose(fd);
			return 0;
		}
		lReadSize = gzread(fd, rom_image, MAX_ROM_SIZE);
		gzclose(fd);
		break;

	}case EXT_ZIP:	// "zip"
		if (path_inzip[0]){
			if (getExtId(path_inzip)!=EXT_GB)
				return 0;
			p = strrchr(path_inzip, '/');
			if (!p)
				p = path_inzip;
			else
				p++;
			strcpy(name, p);
		}else{
			p = strrchr(szRomPath, '/');
			if (!p)
				return 0;
			strcpy(name, p+1);
		}
			

		stRomInfo.p_rom_image = rom_image;
		stRomInfo.rom_size = 0;
		memset(stRomInfo.szFileName, 0x00, sizeof(stRomInfo.szFileName));
		// Unzip�R�[���o�b�N�Z�b�g
		Unzip_setCallback(funcUnzipCallback);
		// Unzip�W�J����
	    nRet = Unzip_execExtract(szRomPath, (unsigned long)&stRomInfo);
		if (nRet != UZEXR_OK) {
			// �ǂݍ��ݎ��s�I - ���̃R�[�h�ł́AUZEXR_CANCEL�������ɗ���
			// ���܂����R�[���o�b�N�ŃL�����Z�����ĂȂ��̂Ŗ���
			lReadSize = 0;
			pgFillvram(RGBA_8(255,0,0));
			pgPrintf(0,0,0xFFFF, "Unzip fatal error.");
			pgScreenFlip();
		}
		lReadSize = stRomInfo.rom_size;
		break;*/
	default:
		return 0;
	}
	
	if(lReadSize){
		strcpy(RomName, name);
		p = strrchr(RomName, '.');
		if(p)
			*p = 0;
	}
	
	return lReadSize;
}