struct treestack;

struct treestack * newstack();
void pushtree(struct treestack *stack, struct ast *tree);
struct ast * poptree(struct treestack *stack);
bool isempty(struct treestack *stack);
