
extern int wavout_enable;
extern unsigned long cur_play;
extern unsigned int nextbank;

//�T�E���h�o�b�t�@�P�o���N������̗e�ʁB�S�o���N�œK���Ƀ��E���h���r��
//PGA_SAMPLES�̔{���ɂ��邱�ƁBPGA_SAMPLES�Ɠ������Ƒ����_���Ȃ̂Œ��ӁB - LCK

//Attention: the requestchunk size must fit into of banks * sound_buff_len
#define SOUND_BUF_LEN 1024

#define BANKS 2
extern short * sound_buf;

void wavoutClear();
int wavoutInit();

