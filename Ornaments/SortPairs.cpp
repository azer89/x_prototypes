std::vector<std::pair<int, float>> pairs;
for (int a = 0; a < 100; a++)
{
	std::pair<int, float> aPair(a, _cvWrapper.GetRandom(0, 100));
	pairs.push_back(aPair);
}

std::sort(pairs.begin(), pairs.end(), [](const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return left.second < right.second;
});

for (int a = 0; a < pairs.size(); a++)
{
	std::cout << pairs[a].first << "\t\t" << pairs[a].second << "\n";
}
