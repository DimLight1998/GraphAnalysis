import uuid
from random import randint as ri
from random import random as rdm


def get_a_random_name() -> str:
    """
    :return: A random name.
    """
    return uuid.uuid4().hex


def generate_fake_graph_simple(num_node: int, num_category: int, prob: float, min_conn: int, max_conn: int)->None:
    """
    :param num_node:
    :param num_category:
    :param prob:
    :param min_conn:
    :param max_conn:
    :return:
    """
    print('Generating fake movie info...')
    all_movies = []
    for i in range(num_node):
        all_movies.append((get_a_random_name(), ri(1, num_category)))

    print('Generating fake movie connection...')
    movies_conn = {}
    for i in range(num_node):
        for j in range(i + 1, num_node):
            if rdm() < prob:
                movies_conn[(all_movies[i][0], all_movies[j][0])
                            ] = ri(min_conn, max_conn)

    print('Writing fake movie info...')
    with open('../Build/Assets/movie.csv', 'w+') as f:
        for movie in all_movies:
            f.write(f'{movie[1]},{movie[0]},{"%.1f" % (10 * rdm())}\n')

    print('Writing fake user info...')
    with open('../Build/Assets/user.csv', 'w+') as f:
        for conn in movies_conn:
            for i in range(movies_conn[conn]):
                user_name = get_a_random_name()
                f.write(f'{conn[0]},{user_name}\n')
                f.write(f'{conn[1]},{user_name}\n')


def generate_fake_graph_from_file(file_name: str)->None:
    with open(file_name, 'r') as f:
        all_movies = []

        num_movie = int(f.readline())
        for i in range(num_movie):
            all_movies.append(f.readline().split('\n')[0])

        with open('../Build/Assets/movie.csv', 'w+') as mf:
            for movie in all_movies:
                mf.write(f'0,{movie},{"%.1f" % (10 * rdm())}\n')

        with open('../Build/Assets/user.csv', 'w+') as uf:
            num_conn = int(f.readline())
            for i in range(num_conn):
                conn = f.readline()
                conn = conn.split()

                for j in range(int(conn[2])):
                    user_name = get_a_random_name()
                    uf.write(f'{conn[0]},{user_name}\n')
                    uf.write(f'{conn[1]},{user_name}\n')

if __name__ == '__main__':
    generate_fake_graph_from_file('../Build/Assets/from.txt')
