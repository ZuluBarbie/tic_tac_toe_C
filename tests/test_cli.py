"""Black-box terminal regression tests; standard-library Python only."""
import subprocess
import sys
import unittest

EXE = sys.argv.pop(1)

def play(text):
    p = subprocess.run([EXE], input=text, text=True, capture_output=True, timeout=10)
    assert p.returncode == 0, p.stderr
    return p.stdout

class TerminalTests(unittest.TestCase):
    def test_x_win_and_replay_scores(self):
        out = play('1\n1\n4\n2\n5\n3\n1\n1\n4\n2\n5\n3\nq\n')
        self.assertEqual(out.count('Player 1 (X) wins!'), 2)
        self.assertIn('Player 1 (X): 2', out)

    def test_o_win(self):
        out = play('1\n1\n4\n2\n5\n9\n6\nq\n')
        self.assertIn('Player 2 (O) wins!', out)
        self.assertIn('Player 2 (O): 1', out)

    def test_draw(self):
        out = play('1\n1\n2\n3\n5\n4\n6\n8\n7\n9\nq\n')
        self.assertIn("It's a draw!", out)
        self.assertIn('Draws: 1', out)

    def test_invalid_input_and_occupied(self):
        invalid = '\nabc\n0\n10\n-1\n1.5\n1x\n1 2\n' + '9' * 500 + '\n\x00\n'
        out = play(invalid + ' 1 \n' + invalid + '1\n1\n4\n2\n5\n3\nq\n')
        self.assertEqual(out.count('Please enter one number'), 20)
        self.assertIn('That square is occupied', out)
        self.assertIn('Player 1 (X) wins!', out)

    def test_eof_and_quit(self):
        for text in ['', '1\n', '2\n', 'garbage', '1\n1', 'Q', '1\nq\n', 'x' * 500]:
            with self.subTest(text=text[:20]):
                self.assertIn('Thanks for playing!', play(text))

    def test_mode_switch_and_ai(self):
        out = play('1\n1\n4\n2\n5\n3\n2\n2\n1\nq\n')
        self.assertIn('Computer chooses 5.', out)
        self.assertIn('Player 1 (X): 1', out)
        self.assertIn('Computer (O): 0', out)

    def test_ninth_move_win(self):
        out = play('1\n1\n2\n5\n3\n6\n4\n7\n8\n9\nq\n')
        self.assertIn('Player 1 (X) wins!', out)
        self.assertNotIn("It's a draw!", out)

if __name__ == '__main__':
    unittest.main()
