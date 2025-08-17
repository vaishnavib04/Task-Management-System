#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define TASKS_FILE "tasks.txt"

typedef struct {
    int id;
    char title[100];
    int completed;  // 0 = incomplete, 1 = complete
} Task;


static Task tasks[MAX_TASKS];
static int taskCount = 0;
static GtkWidget *list_box;


static void load_tasks_from_file(void);
static void save_tasks_to_file(void);
static void display_tasks(void);
static void on_add_clicked(GtkButton *button, gpointer entry);
static void on_complete_toggled(GtkToggleButton *button, gpointer task_ptr);
static void on_edit_clicked(GtkButton *button, gpointer task_ptr);
static void on_delete_clicked(GtkButton *button, gpointer task_ptr);


static void load_tasks_from_file(void) {
    FILE *file = fopen(TASKS_FILE, "r");
    if (!file) {
        g_print("[load] No existing tasks file found, starting fresh.\n");
        taskCount = 0;
        return;
    }

    taskCount = 0;
    char line[256];
    while (taskCount < MAX_TASKS && fgets(line, sizeof(line), file)) {
        
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            len--;
            if (len > 0 && line[len-1] == '\r')  
                line[len-1] = '\0';
        }

        
        int id = 0, completed = 0;
        char title[100];
        int scanned = sscanf(line, "%d|%99[^|]|%d", &id, title, &completed);
        if (scanned == 3) {
            tasks[taskCount].id = id;
            strncpy(tasks[taskCount].title, title, sizeof(tasks[taskCount].title)-1);
            tasks[taskCount].title[sizeof(tasks[taskCount].title)-1] = '\0';
            tasks[taskCount].completed = completed;
            g_print("[load] Loaded task id=%d title=\"%s\" completed=%d\n", id, title, completed);
            taskCount++;
        } else {
            g_print("[load] Skipping invalid line: \"%s\"\n", line);
        }
    }

    fclose(file);
}


static void save_tasks_to_file(void) {
    FILE *file = fopen(TASKS_FILE, "w");
    if (!file) {
        g_print("[save] ERROR: Unable to open tasks file for writing.\n");
        return;
    }
    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d|%s|%d\n", tasks[i].id, tasks[i].title, tasks[i].completed);
    }
    fclose(file);
    g_print("[save] Tasks saved successfully (%d tasks).\n", taskCount);
}

static void display_tasks(void) {
    
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    g_print("[display] Showing %d tasks\n", taskCount);


    for (int i = 0; i < taskCount; i++) {
        Task *task = &tasks[i];

        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_container_add(GTK_CONTAINER(row), hbox);

        GtkWidget *check = gtk_check_button_new();
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), task->completed);
        g_signal_connect(check, "toggled", G_CALLBACK(on_complete_toggled), task);

        GtkWidget *label = gtk_label_new(task->title);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_widget_set_hexpand(label, TRUE);

        if (task->completed) {
            PangoAttrList *attrs = pango_attr_list_new();
            pango_attr_list_insert(attrs, pango_attr_strikethrough_new(TRUE));
            gtk_label_set_attributes(GTK_LABEL(label), attrs);
            pango_attr_list_unref(attrs);
        } else {
            gtk_label_set_attributes(GTK_LABEL(label), NULL);
        }

        GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(on_edit_clicked), task);

        GtkWidget *del_btn = gtk_button_new_with_label("Delete");
        g_signal_connect(del_btn, "clicked", G_CALLBACK(on_delete_clicked), task);

        gtk_box_pack_start(GTK_BOX(hbox), check, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), edit_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), del_btn, FALSE, FALSE, 0);

        gtk_container_add(GTK_CONTAINER(list_box), row);
    }

    gtk_widget_show_all(list_box);
}


static void on_complete_toggled(GtkToggleButton *button, gpointer task_ptr) {
    Task *task = (Task *)task_ptr;
    task->completed = gtk_toggle_button_get_active(button);
    save_tasks_to_file();
    display_tasks();
}

static void on_edit_clicked(GtkButton *button, gpointer task_ptr) {
    Task *task = (Task *)task_ptr;
    GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(button));

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Edit Task",
                                                    GTK_WINDOW(toplevel),
                                                    GTK_DIALOG_MODAL,
                                                    "_Save", GTK_RESPONSE_ACCEPT,
                                                    "_Cancel", GTK_RESPONSE_REJECT,
                                                    NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), task->title);
    gtk_container_add(GTK_CONTAINER(content), entry);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char *new_title = gtk_entry_get_text(GTK_ENTRY(entry));
        if (new_title && strlen(new_title) > 0) {
            strncpy(task->title, new_title, sizeof(task->title) - 1);
            task->title[sizeof(task->title) - 1] = '\0';
            save_tasks_to_file();
            display_tasks();
        }
    }

    gtk_widget_destroy(dialog);
}


static void on_delete_clicked(GtkButton *button, gpointer task_ptr) {
    Task *task = (Task *)task_ptr;

    int idx = -1;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == task->id) {
            idx = i;
            break;
        }
    }

    if (idx == -1) return;

    for (int i = idx; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    taskCount--;
    save_tasks_to_file();
    display_tasks();
}

static void on_add_clicked(GtkButton *button, gpointer entry) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    if (!text || strlen(text) == 0 || taskCount >= MAX_TASKS) return;

    
    int max_id = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id > max_id) max_id = tasks[i].id;
    }
    tasks[taskCount].id = max_id + 1;

    strncpy(tasks[taskCount].title, text, sizeof(tasks[taskCount].title) - 1);
    tasks[taskCount].title[sizeof(tasks[taskCount].title) - 1] = '\0';
    tasks[taskCount].completed = 0;
    taskCount++;

    save_tasks_to_file();
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    display_tasks();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "To-Do List");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *add_btn = gtk_button_new_with_label("Add Task");
    gtk_box_pack_start(GTK_BOX(entry_box), entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(entry_box), add_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), entry_box, FALSE, FALSE, 0);

    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled), list_box);
    gtk_box_pack_start(GTK_BOX(main_box), scrolled, TRUE, TRUE, 0);

    g_signal_connect(add_btn, "clicked", G_CALLBACK(on_add_clicked), entry);

    load_tasks_from_file();
    display_tasks();

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

