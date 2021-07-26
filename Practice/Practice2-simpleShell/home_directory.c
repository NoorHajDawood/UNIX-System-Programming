#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

int main(int argc, char const *argv[])
{
    uid_t uid = getuid();
    struct passwd* pwd = getpwuid(uid);

    if (!pwd)
    {
        printf("User with %u ID is unknown.\n", uid);
        return -1;
    }

    printf("User '%s' has home directory at '%s'\n", pwd->pw_name, pwd->pw_dir);

    if (chdir(pwd->pw_dir) != 0)
    {
        printf("Unable to change directory to '%s'\n", pwd->pw_dir);
        return -1;
    }
    return 0;
}
