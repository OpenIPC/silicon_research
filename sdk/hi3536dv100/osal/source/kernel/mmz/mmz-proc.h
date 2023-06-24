#include <linux/proc_fs.h>
#include <linux/seq_file.h>

extern int mmz_read_proc(struct seq_file *sfile);

static void *mmz_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter;
	if (*pos == 0)
		return &counter;
	else {
		*pos = 0;
		return NULL;
	}
}

static void *mmz_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return NULL;
}

static void mmz_seq_stop(struct seq_file *s, void *v)
{
}

static int mmz_seq_show(struct seq_file *sfile, void *v)
{
	return mmz_read_proc(sfile);
}
