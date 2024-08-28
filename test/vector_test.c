#include "vector.h"
#include <stdbool.h>
#include <stdio.h>

int exit_code     = 0;
int total_checks  = 0;
int passed_checks = 0;

void check(bool cond, char* description) {
  total_checks++;

  if (!cond) {
    printf("check failed: %s\n", description);
    exit_code = 1;

    return;
  }

  printf("check passed: %s\n", description);
  passed_checks++;
}

int main() {
  check(equal3(cross(i_hat3, j_hat3), k_hat3), "cross identity");
  check(equal3(cross(j_hat3, i_hat3), scale3(-1, k_hat3)), "cross identity 2");
  check(
      equal3(
          cross((vector3){1, 2, 3}, (vector3){-71, 9, -4}),
          (vector3){-35, -209, 151}
      ),
      "cross random vectors"
  );

  printf("%d/%d checks passed\n", passed_checks, total_checks);

  return exit_code;
}
