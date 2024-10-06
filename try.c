#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void	main(void)

{
	int i;

	signal(SIGINT, SIG_IGN);
	/* ignore the signal */

	printf("I'm running ...\n");

	for (i = 1; i < 10; i++)

	{
		printf("Still going ...\n");

		sleep(1);

		if (i == 3)
        {
            printf("%d\n",i);
			signal(SIGINT, SIG_DFL);
        }
		/* re-install default handler */
	}
}