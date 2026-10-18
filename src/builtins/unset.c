#include "minishell.h"

static char	**remove_env_variable(char **envp, int index, t_minishell *data)
{
	char	**new_envp;
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (data->envp[++i])
		set_gc_node_locked(&data->gc, data->envp[i], false);
	set_gc_node_locked(&data->gc, data->envp, false);
	new_envp = gc_malloc(sizeof(char *) * ft_tabstrlen(envp), &data->gc);
	set_gc_node_locked(&data->gc, new_envp, true);
	i = -1;
	while (envp[++j])
	{
		set_gc_node_locked(&data->gc, envp[j], false);
		if (j != index)
		{
			new_envp[++i] = ft_strdup_gc(envp[j], &data->gc);
			set_gc_node_locked(&data->gc, new_envp[i], true);
		}
	}
	new_envp[i] = NULL;
	return (new_envp);
}

void	ft_unset(t_node *cmd_args, t_minishell *data)
{
	int		index;
	char	**new_envp;

	while (cmd_args)
	{
		index = get_env_index(cmd_args->value, data);
		if (index != -1)
		{
			new_envp = remove_env_variable(data->envp, index, data);
			free_envp(data);
			data->envp = new_envp;
		}
		cmd_args = cmd_args->next;
	}
	data->last_exit_status = 0;
}
