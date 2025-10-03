#include "simplefind.h"
#include <dirent.h>

int print_entry(char *pattern, char *pathname, bool verbose)
{
    // checking for pattern match
    if (pattern != NULL && fnmatch(pattern, basename(pathname), 0) != 0) // basename function is AWESOME
    {
        return 0;
    }

    struct stat path_stat;
    if (lstat(pathname, &path_stat) == -1)
    {
        return -1;
    }
    if (verbose)
    {

        char time[64];
        struct tm *local = localtime(&(path_stat.st_mtime));
        if (local == NULL)
        {
            errno = EINVAL; // time conversion error
            return -1;
        }

        strftime(time, sizeof(time), "%b %d %H:%M", local);
        struct passwd *user;
        struct group *group;
        user = getpwuid(path_stat.st_uid);
        if (user == NULL)
        {
            errno = EINVAL; // user ID conversion error
            return -1;
        }
        group = getgrgid(path_stat.st_gid);
        if (group == NULL)
        {
            errno = EINVAL; // same as before
            return -1;
        }
        char display_path[1024];
        char target[1024];
        if (S_ISLNK(path_stat.st_mode)) {
            int len = readlink(pathname, target, sizeof(target)-1);
            if (len != -1) {
                target[len] = '\0';
                snprintf(display_path, sizeof(display_path), "%s -> %s", pathname, target);
            } else {
                strncpy(display_path, pathname, sizeof(display_path));
            }
        } else {
            strncpy(display_path, pathname, sizeof(display_path));
        }
        display_path[sizeof(display_path)-1] = '\0';
        // mode parsing
        char* modes = "rwxrwxrwx";
        char mode_str[11];
        switch (path_stat.st_mode & S_IFMT) { // S_IFMT is a bitmask to get file type
            case S_IFREG: mode_str[0] = '-'; break;
            case S_IFDIR: mode_str[0] = 'd'; break;
            case S_IFLNK: mode_str[0] = 'l'; break;
            case S_IFCHR: mode_str[0] = 'c'; break;
            case S_IFBLK: mode_str[0] = 'b'; break;
            case S_IFSOCK: mode_str[0] = 's'; break;
            case S_IFIFO: mode_str[0] = 'p'; break;
            default: mode_str[0] = '?'; break;
        }
        for (int i = 1; i < 10; i++) { // first char is file type, next 8 are permissions
            if (path_stat.st_mode & (1 << (9 - i))) {
                mode_str[i] = modes[i - 1];
            } else {
                mode_str[i] = '-';
            }
        }
        mode_str[10] = '\0';


        printf("%d  %d %s %d %s %s %d %s %s\n", path_stat.st_ino, path_stat.st_blocks*2, mode_str, path_stat.st_nlink, user->pw_name, group->gr_name, path_stat.st_size, time, display_path);
    } // note that st_blocks is in 512-byte blocks, and find usually shows in 1K blocks, so we multiply by 2
    else
    {
        printf("%s\n", pathname);
    }
    return 0;
}

int traverse(char *pattern, char *path, bool verbose, bool x, dev_t start_dev)
{
    DIR *directory = opendir(path);
    if (directory == NULL)
    {
        return 0; // if we can't open directory, we skip it. this is not an error so we return 0
    }
    struct dirent *dir_entry;
    if (x == true)
    { // if xdev true, check device ID of current directory
        struct stat path_stat;
        if (stat(path, &path_stat) == -1)
        {
            closedir(directory);
            return -1;
        }
        dev_t current = path_stat.st_dev;
        if (start_dev != current)
        {
            closedir(directory);
            return 0; // if device IDs dont match, we skip this directory. this is not an error so we return 0
        }
    }

    // now we use readdir to read contents of directory
    while ((dir_entry = readdir(directory)) != NULL)
    { // once directory is empty (meaning we've looked through it, we can return from the function
        char new_path[1024];
        if (strcmp(path, "/") == 0) snprintf(new_path, sizeof(new_path), "/%s", dir_entry->d_name);
        else snprintf(new_path, sizeof(new_path), "%s/%s", path, dir_entry->d_name);
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
        {
            continue;
        }
        else if (dir_entry->d_type == DT_DIR) // on my end, code compiles but always a squiggly underline here. not sure why since its valid and header included. maybe false err
        {
            // recursively call traverse on this directory
            int result = traverse(pattern, new_path, verbose, x, start_dev);
            if (result == -1)
            {
                continue;
            }

            int print_result = print_entry(pattern, new_path, verbose);
            if (print_result == -1)
            {
                return -1; // -1 in print entry is an actual error
            }
        }
        else
        { // base case is file or symlink
            int print_result = print_entry(pattern, new_path, verbose);
            if (print_result == -1)
            {
                errno = EIO; // io error (i think?)
                return -1;
            }
        }
    }
    return 0;
}
