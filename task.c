#include <gtk/gtk.h>		//控件标准库
#include <stdio.h>		//
//#include <time.h>
#include <string.h>		//strcpy
#include <stdlib.h>		//atoi
#include <dirent.h>		//readir

/*===========================全局变量======================================*/
struct System_info {
	GtkWidget *label_vendor_id;
	GtkWidget *label_model_name;
	GtkWidget *label_cpu_mhz;
	GtkWidget *label_cache_size;
	GtkWidget *label_address_sizes;

	GtkWidget *label_mem_total;

	GtkWidget *label_kernel_version;
	GtkWidget *label_host_name;
	GtkWidget *label_os_version;
} system_info;

GtkTextBuffer *device_text;
GtkTextBuffer *progress_text;

int notebook_pos;

struct Resource_info {
	GtkCurve *cpu_resource_curve;
	float vector[500];
	long idle_time, cpu_time;
	GtkProgressBar *mem_progress_bar;
} resource_info;

struct Status_bar {
	GtkLabel *cpu;
	GtkLabel *mem;
	GtkLabel *time;
} status_bar;


/*===========================file菜单======================================*/

void on_log_out_activate(GtkObject * object, gpointer user_data)
{
	system("logout");
}

void on_restart_activate(GtkObject * object, gpointer user_data)
{
	system("reboot");
}

void on_halt_activate(GtkObject * object, gpointer user_data)
{
	system("halt");
}

void on_window_destroy(GtkObject * object, gpointer user_data)
{				//退出主函数
	gtk_main_quit();
}

/*===========================视图菜单======================================*/
void on_rotate_activate(GtkObject *object, gpointer usr_data)
{
    gtk_notebook_set_tab_pos( (GtkNotebook*)object, (((GtkNotebook*)object)->tab_pos + 1) % 3);
}

/*===========================杀死进程菜单======================================*/

void on_progress_activate(GtkObject *object, gpointer usr_data)
{
    gtk_widget_show((GtkWidget *) object);
}

void on_kill_cancel_clicked(GtkObject *object, gpointer usr_data)
{
    gtk_widget_hide((GtkWidget *) object);
}


void on_kill_dialog_destroy(GtkObject *object, gpointer usr_data)
{
    gtk_widget_hide((GtkWidget *) object);
}

void on_kill_ok_clicked(GtkObject *object, gpointer usr_data)
{
    //char *progress_name;
    char order[16];
    
    strcpy(order, "kill -9 ");
    strcat(order, gtk_entry_get_text((GtkEntry *) object));
    //printf("%s",s);
    system(order);//printf("%s\n", gtk_entry_get_text((GtkEntry *) object));
     
}
/*===========================about菜单======================================*/

void on_about_activate(GtkObject * object, gpointer user_data)
{
	gtk_widget_show((GtkWidget *) object);
}

void on_about_exit_clicked(GtkObject * object, gpointer user_data)
{
	gtk_widget_hide((GtkWidget *) object);
}

 /*===========================system页======================================*/

void get_system_cpu_info()
{
	FILE *fp;
	char buffer[256];
	int i;
	int num = 0;

	fp = fopen("/proc/cpuinfo", "r");
	while ((fgets(buffer, 256, fp)) && (num < 27)) {

		switch (num) {
		case 1:
			i = strlen(buffer) - 1;
			buffer[72] = '\0';
			while (i < 72) {
				buffer[i] = ' ';
				i++;
			}	//printf("%d\n%s\n",strlen(buffer), buffer);
			gtk_label_set_text((GtkLabel *)
					   system_info.label_vendor_id, buffer);
			break;
		case 4:
			i = strlen(buffer) - 1;
			buffer[67] = '\0';
			while (i < 67) {
				buffer[i] = ' ';
				i++;
			}	//printf("%d\n%s\n",strlen(buffer), buffer);
			gtk_label_set_text((GtkLabel *) system_info.
					   label_model_name, buffer);
			break;
		case 6:
			i = strlen(buffer) - 1;
			buffer[62] = '\0';
			while (i < 62) {
				buffer[i] = ' ';
				i++;
			}	//printf("%d\n%s\n",strlen(buffer), buffer);
			gtk_label_set_text((GtkLabel *) system_info.
					   label_cpu_mhz, buffer);
			break;
		case 7:
			i = strlen(buffer) - 1;
			buffer[72] = '\0';
			while (i < 72) {
				buffer[i] = ' ';
				i++;
			}	//printf("%d\n%s\n",strlen(buffer), buffer);
			gtk_label_set_text((GtkLabel *)
					   system_info.label_cache_size,
					   buffer);
			break;
		case 26:
			i = strlen(buffer) - 1;
			buffer[78] = '\0';
			while (i < 78) {
				buffer[i] = ' ';
				i++;
			}	//printf("%d\n%s\n",strlen(buffer), buffer);
			gtk_label_set_text((GtkLabel *)
					   system_info.label_address_sizes,
					   buffer);
			break;
		}
		num++;
	}
	fclose(fp);
}

void get_system_mem_info()
{
	FILE *fp;
	char buffer[80];
	int i;

	fp = fopen("/proc/meminfo", "r");
	fgets(buffer, 80, fp);
	i = strlen(buffer) - 1;	//printf("%c", buffer[i]);
	buffer[73] = '\0';
	while (i < 73) {
		buffer[i] = ' ';
		i++;
	}			//printf("%d\n%s\n",strlen(buffer), buffer);
	gtk_label_set_text((GtkLabel *) system_info.label_mem_total, buffer);

	fclose(fp);
}

void get_system_os_info()
{
	FILE *fp;
	char buffer[256];
	char tmp[80];
	char *tmp2;
	char tmp3[80];
	int i;

	fp = fopen("/proc/version", "r");
	fgets(buffer, 256, fp);
	fclose(fp);

	tmp2 = strtok(buffer, "(");

	strcpy(tmp3, tmp2);
	strcpy(tmp, "kernel version: ");
	strcat(tmp, tmp3);
	i = strlen(tmp);
	tmp[77] = '\0';
	while (i < 77) {
		tmp[i] = ' ';
		i++;
	}			//printf("%d\n%s\n",strlen(buffer), buffer);
	gtk_label_set_text((GtkLabel *) system_info.label_kernel_version, tmp);

	tmp2 = strtok(NULL, ")");

	strcpy(tmp3, tmp2);
	strcpy(tmp, "host name    : ");
	strcat(tmp, tmp3);
	i = strlen(tmp);
	tmp[74] = '\0';
	while (i < 74) {
		tmp[i] = ' ';
		i++;
	}			//printf("%d\n%s\n",strlen(buffer), buffer);
	gtk_label_set_text((GtkLabel *) system_info.label_host_name, tmp);

	tmp2 = strtok(NULL, "(");
	tmp2 = strtok(NULL, "(");

	strcpy(tmp3, tmp2);
	strcpy(tmp, "os kernel     : ");
	strcat(tmp, tmp3);
	i = strlen(tmp);
	tmp[76] = '\0';
	while (i < 76) {
		tmp[i] = ' ';
		i++;
	}			//printf("%d\n%s\n",strlen(buffer), buffer);
	gtk_label_set_text((GtkLabel *) system_info.label_os_version, tmp);

}

int get_system_page_info(void)
{
	// g_source_remove(system_page_timer);

	get_system_cpu_info();
	get_system_mem_info();
	get_system_os_info();

	return 0;
}
 /*===========================device页======================================*/

int get_device_page_info(void)
{
	FILE *fp;
	char buffer[4096];
	int length;

	//g_source_remove(device_page_timer);

	fp = fopen("/proc/devices", "r");
	length = fread(buffer, sizeof(char), 4096, fp);
	fclose(fp);

	gtk_text_buffer_set_text(device_text, buffer, length);

	return 1;
}


/*===========================resource页======================================*/

void get_resource_cpu_info(void)
{				//user, nice, system, idle
	FILE *fp;
	//float cpu_rate;
	long cal[7];
	//char *tmp;
	char buffer[80];	//设置成数组定时器不能用
	char label_status_cpu[4];
	char label_status_cpu_final[10];
	int tmp2;
//*
	int i = 499, j = 0;
//int i= 49, j = 0;

	fp = fopen("/proc/stat", "r");
	// i , j;
	//fp = fopen("/proc/stat", "r");
	fgets(buffer, 80, fp);

	strtok(buffer, " ");
	// tmp = strtok (NULL, " ");
	while (cal[j] = atoi(strtok(NULL, " "))) {	//printf("%d\n", cal[j]);
		j++;
	}
	//cpu_rate = (cal[3]-idle_time)/(cal[0]+cal[1]+cal[2]+cal[3]+cal[4]+cal[5]+cal[6] - cpu_time);

	while (i > 0) {
		resource_info.vector[i] = resource_info.vector[i - 1];
		i--;
	}
	resource_info.vector[0] =
	    1 - 0.1 * (cal[3] - resource_info.idle_time) / (cal[0] + cal[1] +
							    cal[2] + cal[3] +
							     - resource_info.
							    cpu_time) * 10;
	//printf("%f\n", resource_info.vector[0]);
	gtk_curve_set_vector(resource_info.cpu_resource_curve, 500,
			     resource_info.vector);

	tmp2 = (int)(resource_info.vector[0] * 100);
	if ((resource_info.vector[0] * 100 - tmp2) >= 0.5)
		tmp2 += 1;

	label_status_cpu[0] = tmp2 / 100 + '0';
	tmp2 = tmp2 - tmp2 / 100 * 100;
	label_status_cpu[0] = tmp2 / 10 + '0';
	tmp2 = tmp2 - tmp2 / 10 * 10;
	label_status_cpu[1] = tmp2 + '0';
	label_status_cpu[2] = '\0';	//printf("%s\n", label_status_cpu);

	strcpy(label_status_cpu_final, "CPU:");
	strcat(label_status_cpu_final, label_status_cpu);
	strcat(label_status_cpu_final, "%");
	gtk_label_set_text(status_bar.cpu, label_status_cpu_final);

	resource_info.idle_time = cal[3];
	resource_info.cpu_time =cal[0] + cal[1] + cal[2] + cal[3] ;
	   // cal[0] + cal[1] + cal[2] + cal[3] + cal[4] + cal[5] + cal[6];

	fclose(fp);		//*/
}

//itoa不可用
void get_resource_mem_info(void)
{
	FILE *fp;
	long all, free;
	char *tmp;
	char buffer[80];
	char label_status_mem[4];
	char label_status_mem_final[10];
	int tmp2;

	fp = fopen("/proc/meminfo", "r");
	fgets(buffer, 80, fp);
	tmp = strtok(buffer, " ");
	tmp = strtok(NULL, " ");
	all = atoi(tmp);

	fgets(buffer, 80, fp);
	tmp = strtok(buffer, " ");
	tmp = strtok(NULL, " ");
	free = atoi(tmp);
	fclose(fp);
	//printf("%ld,,,%ld,,,%f,,,\n", all, free, 0.1*(all-free)/all);
	gtk_progress_bar_set_fraction(resource_info.mem_progress_bar,
				      0.1 * (all - free) / all * 10);

	tmp2 = (int)(0.1 * (all - free) / all * 1000);
	if ((0.1 * (all - free) / all * 1000 - tmp2) >= 0.5)
		tmp2 += 1;

	label_status_mem[0] = tmp2 / 100 + '0';
	tmp2 = tmp2 - tmp2 / 100 * 100;
	label_status_mem[0] = tmp2 / 10 + '0';
	tmp2 = tmp2 - tmp2 / 10 * 10;
	label_status_mem[1] = tmp2 + '0';
	label_status_mem[2] = '\0';

	strcpy(label_status_mem_final, "MEM:");
	strcat(label_status_mem_final, label_status_mem);
	strcat(label_status_mem_final, "%");
	gtk_label_set_text(status_bar.mem, label_status_mem_final);
}

int get_resource_page_info(void)
{
	get_resource_mem_info();
	get_resource_cpu_info();

	return 1;
}
/*=================================now time===============================================*/
int get_now_time(void)
{
	FILE *fp;
	char buffer[40];
	char *tmp;
	char time[15];

	fp = fopen("/proc/driver/rtc", "r");
	fgets(buffer, 40, fp);
	fclose(fp);

	strtok(buffer, " ");
	tmp = strtok(NULL, "\n");

	strcpy(time, "time: ");
	strcat(time, tmp);
	gtk_label_set_text(status_bar.time, time);

	return 1;
}


/*===========================progress页======================================*/
int is_progress(char file_name[])
{				
	int i = 0, j = strlen(file_name);
	//printf("%d\t%s\t%c\n", i, file_name, file_name[0]);
	while (i < j) {
		if (('0' > file_name[i]) || (file_name[i] > '9'))
			return 0;
		i++;
	}
	return 1;
}
char buffer_progress[10440];
 
int get_progress_page_info(void)
{
	DIR *dir;
	struct dirent *pro_dir;
	FILE *fp;
	char file_name[20];
	char file_buffer[80];
	
	char item_pro[48];
	char pid_tmp[6];
	char *tmp;
	int num;
//buffer[0] = '\t';
    buffer_progress[0] = '\0';
	dir = opendir("/proc");
	while ((pro_dir = readdir(dir)) != NULL) 
	{
		if (is_progress(pro_dir->d_name)) 
		{
		    //printf("%s\n", pro_dir->d_name);
		
			strcpy(file_name, "/proc/");
			strcat(file_name, pro_dir->d_name);
			strcat(file_name, "/status");
			
			fp = fopen(file_name, "r");
			num = 0;
			while ((fgets(file_buffer, 40, fp)) && (num < 15)) 
			{
				
				switch (num) 
				{
				case 0://15
				 	strtok(file_buffer, "\t");//如果是" ", 那么就可能出现不对齐的情况
					tmp =strtok(NULL, "\n");
					sprintf(item_pro, "%18s", tmp);
					break;
					
				case 1://1
					strtok(file_buffer, "\t");
					tmp = strtok(NULL, " ");
					strcat(item_pro, "\t");
					sprintf(pid_tmp, "%1s", tmp);
					strcat(item_pro, pid_tmp);
					break;
					
				case 3://4
					strtok(file_buffer, "\t");//pid 可能是5位的阿
					tmp = strtok(NULL, "\n");
					strcat(item_pro, "\t");
					sprintf(pid_tmp, "%5s", tmp);
					strcat(item_pro, pid_tmp);//printf("%s\n", item_pro);
					//printf("--%s\t--%d\n",  tmp, strlen(tmp));
					break;
					
				case 4://4
					strtok(file_buffer, "\t");
					tmp = strtok(NULL, "\n");
					strcat(item_pro, "\t");
					sprintf(pid_tmp, "%5s", tmp);
					strcat(item_pro, pid_tmp);
				//	printf("--%s\t--%d\n",  tmp, strlen(tmp));
					break;
					
				case 14://11
					strtok(file_buffer, "\t");
					tmp = strtok(NULL, "\n");
					if(tmp[0]=='0') {
					    tmp[0] = '\0';
					    //tmp[1] = '\0';
					}//printf("--%s\t--%d\n",  tmp, strlen(tmp));
					strcat(item_pro, "\t");
					sprintf(pid_tmp, "%11s", tmp);
					strcat(item_pro, pid_tmp);
					strcat(item_pro, "\n");
				
					
                    strcat(buffer_progress, item_pro);
				}//switch/**/
				//printf("i am ok..%s\n", file_buffer);
				num++;
				
			}//while
            fclose(fp);
		}//if
	}
	closedir(dir);
	//printf("%s", buffer_progress);
	//printf("hello\n");
	tmp = buffer_progress;
	gtk_text_buffer_set_text(progress_text, tmp, strlen(buffer_progress));
	return 1;
}
      

/*===========================time页======================================*/
int refresh_ui(void)
{
    get_progress_page_info();
    get_now_time();
    get_resource_page_info();
    get_device_page_info();
    get_system_page_info();
    
    return 0;
}
/*=================================main===============================================*/

int main(int argc, char *argv[])
{
	GtkBuilder *builder;
	GtkWidget *window;
	//int resource_page_timer;

	/*
	   GtkWidget * label_vendor_id;
	   GtkWidget * label_model_name;
	   GtkWidget * label_cpu_mhz;
	   GtkWidget * label_cache_size;
	   GtkWidget * label_address_sizes;
	 */
	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "task.xml", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	gtk_widget_set_size_request(GTK_WIDGET(window), 600, 400);
	gtk_window_set_title(GTK_WINDOW(window), "Task Manager");
	gtk_container_set_border_width(GTK_CONTAINER(window), 5);	//设置窗口边框宽度


    g_timeout_add(3, (GtkFunction) refresh_ui, NULL);
    /*===========================system页======================================*/

	system_info.label_vendor_id =
	    GTK_WIDGET(gtk_builder_get_object(builder, "vendor_id"));
	system_info.label_model_name =
	    GTK_WIDGET(gtk_builder_get_object(builder, "model_name"));
	system_info.label_cpu_mhz =
	    GTK_WIDGET(gtk_builder_get_object(builder, "cpu_mhz"));
	system_info.label_cache_size =
	    GTK_WIDGET(gtk_builder_get_object(builder, "cache_size"));
	system_info.label_address_sizes =
	    GTK_WIDGET(gtk_builder_get_object(builder, "address_sizes"));

	system_info.label_mem_total =
	    GTK_WIDGET(gtk_builder_get_object(builder, "mem_total"));

	system_info.label_os_version =
	    GTK_WIDGET(gtk_builder_get_object(builder, "os_version"));
	system_info.label_kernel_version =
	    GTK_WIDGET(gtk_builder_get_object(builder, "kernel_version"));
	system_info.label_host_name =
	    GTK_WIDGET(gtk_builder_get_object(builder, "host_name"));

	//system_page_timer =
	//g_timeout_add(5, (GtkFunction) get_system_page_info, NULL);

    /*===========================device页======================================*/
	device_text =
	    (GtkTextBuffer *) gtk_builder_get_object(builder,
						     "device_text_buffer");

	//device_page_timer =
	g_timeout_add(10000, (GtkFunction) get_device_page_info, NULL);

     /*===========================resource页======================================*/
	resource_info.cpu_resource_curve =
	    (GtkCurve *) gtk_builder_get_object(builder, "cpu_curve");
	resource_info.mem_progress_bar =
	    (GtkProgressBar *) gtk_builder_get_object(builder, "mem_progress");

	//resource_page_timer = 
	g_timeout_add(1000, (GtkFunction) get_resource_page_info, NULL);

    /*===========================status栏======================================*/
	status_bar.cpu =
	    (GtkLabel *) gtk_builder_get_object(builder, "cpu_status_bar");
	status_bar.mem =
	    (GtkLabel *) gtk_builder_get_object(builder, "mem_status_bar");
	status_bar.time =
	    (GtkLabel *) gtk_builder_get_object(builder, "now_time");

	g_timeout_add(1000, (GtkFunction) get_now_time, NULL);
    /*==============================progress项=========================================*/
	progress_text = (GtkTextBuffer *) gtk_builder_get_object(builder,
								 "progress_text_buffer");

	g_timeout_add(15000, (GtkFunction) get_progress_page_info, NULL);

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	gtk_widget_show(window);

	gtk_main();

	return 0;
}
